#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../vendor/stb_image.h"
#include "includes/skybox.h"
const char* format[] = {"top", "bottom", "front", "back", "right", "left"};
const GLenum gl_format[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X};
const float cube_vertices[] = {
  -1.0,  1.0, -1.0,
  -1.0, -1.0, -1.0,
   1.0, -1.0, -1.0,
   1.0, -1.0, -1.0,
   1.0,  1.0, -1.0,
  -1.0,  1.0, -1.0,

  -1.0, -1.0,  1.0,
  -1.0, -1.0, -1.0,
  -1.0,  1.0, -1.0,
  -1.0,  1.0, -1.0,
  -1.0,  1.0,  1.0,
  -1.0, -1.0,  1.0,

   1.0, -1.0, -1.0,
   1.0, -1.0,  1.0,
   1.0,  1.0,  1.0,
   1.0,  1.0,  1.0,
   1.0,  1.0, -1.0,
   1.0, -1.0, -1.0,

  -1.0, -1.0,  1.0,
  -1.0,  1.0,  1.0,
   1.0,  1.0,  1.0,
   1.0,  1.0,  1.0,
   1.0, -1.0,  1.0,
  -1.0, -1.0,  1.0,

  -1.0,  1.0, -1.0,
   1.0,  1.0, -1.0,
   1.0,  1.0,  1.0,
   1.0,  1.0,  1.0,
  -1.0,  1.0,  1.0,
  -1.0,  1.0, -1.0,

  -1.0, -1.0, -1.0,
  -1.0, -1.0,  1.0,
   1.0, -1.0, -1.0,
   1.0, -1.0, -1.0,
  -1.0, -1.0,  1.0,
   1.0, -1.0,  1.0
};
Pipeline_Skybox* Skybox_Create(const Pipeline_Program program, const char* path) {
  Pipeline_Skybox* skybox = calloc(1, sizeof(Pipeline_Skybox));
  FILE* file = File_Open(path, "r");
  skybox->view_matrix_location = Pipeline_GetUniform(program, "viewmatrix");

  char face[16], imagepath[64];
  #define StringsEqual(s1, s2) (!strcmp(s1, s2))
  for(int i = 0; i < 6; ++i) {
    (fscanf(file, "%s %s", face, imagepath) != 2) ?
    App_Fatal("%s format is incorrect, fatal.\n", path) : "Ok.";
    (!StringsEqual(format[i], face)) ?
    App_Fatal("%s face was given instead of %s face, fatal.\n", face, format[i]) : "Ok.";
    skybox->faces[i].data = stbi_load(imagepath, &skybox->faces[i].width, &skybox->faces[i].height, NULL, 3);
    (skybox->faces[i].data == NULL) ?
    App_Fatal("Failed to stbi_load %s, fatal.\n", imagepath) : "Ok.";
    /* Make the imagepathes relative instead of relatively absolute! */
  }

  glGenTextures(1, &skybox->texture);
  /* For now! */
  skybox->texture_unit = Pipeline_NewTextureUnit();
  glActiveTexture(skybox->texture_unit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
  for(int i = 0; i < 6; ++i) {
    glTexImage2D(gl_format[i], 0, GL_RGB, skybox->faces[i].width,
      skybox->faces[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox->faces[i].data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glUniform1i(Pipeline_GetUniform(program, "cube_texture"), skybox->texture_unit - GL_TEXTURE0);

  glGenVertexArrays(1, &skybox->vert_array);
  glGenBuffers(1, &skybox->mesh);
  glBindVertexArray(skybox->vert_array);
  glBindBuffer(GL_ARRAY_BUFFER, skybox->mesh);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

  for(int i = 0; i < 6; ++i) {
    stbi_image_free(skybox->faces[i].data);
  }
  fclose(file);
  return skybox;
}
void Skybox_Render(const Pipeline_Skybox* skybox,
  const Pipeline_Camera* camera, const Pipeline_Program program) {
    glDepthMask(GL_FALSE);
    glUseProgram(program);
    glUniformMatrix4fv(skybox->view_matrix_location, 1, GL_FALSE, camera->rotation.mat);
    glActiveTexture(skybox->texture_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
    glBindVertexArray(skybox->vert_array);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}
void Skybox_Destroy(Pipeline_Skybox** _skybox) {
  /* TODO: fully clean the vertex buffer! */
  if(_skybox == NULL) return;
  Pipeline_Skybox* skybox = *_skybox;
  if(skybox != NULL) {
    free(skybox);
  }
  *_skybox = NULL;
}
