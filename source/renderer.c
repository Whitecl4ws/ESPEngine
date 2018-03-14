#include <stdlib.h>
#include "includes/renderer.h"
/* For now, only one instance and one type of the renderer is possible! */
Pipeline_Instance_Renderer* Pipeline_CreateInstanceRenderer(const Pipeline_Program program,
  const GLuint meshes_cap, const GLuint verts_cap, const GLuint inds_cap) {
  glUseProgram(program);
  #define DEFAULT_POSITION_ATTRIBUTE_LOCATION 0
  Pipeline_CheckAttributeLocation(program, "position", DEFAULT_POSITION_ATTRIBUTE_LOCATION);

  Pipeline_Instance_Renderer* renderer = calloc(1, sizeof(Pipeline_Instance_Renderer));
  renderer->meshes_cap = meshes_cap;
  renderer->verts_cap = verts_cap;
  renderer->inds_cap = inds_cap;
  renderer->mesh_ind_offset = calloc(meshes_cap + 1, sizeof(GLuint));
  glGenVertexArrays(1, &renderer->vert_array);
  glGenBuffers(1, &renderer->verts_buffer);
  glGenBuffers(1, &renderer->inds_buffer);

  glBindVertexArray(renderer->vert_array);
  glBindBuffer(GL_ARRAY_BUFFER, renderer->verts_buffer);
  glBufferData(GL_ARRAY_BUFFER, verts_cap * sizeof(Pipeline_Vertex), NULL, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0); /* These should be fine-tuned! */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Pipeline_Vertex), 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->inds_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds_cap * sizeof(GLuint), NULL, GL_STATIC_DRAW);
  glBindVertexArray(0);
  return renderer;
}
void Instance_Renderer_CreateMesh(Pipeline_Instance_Renderer* renderer, const Pipeline_Vertex* verts,
  const GLuint* indices, const GLuint mesh_verts_count, const GLuint mesh_inds_count) {
  /* The lack of addition is done on purpose :) */
  (renderer->meshes_count > renderer->meshes_cap) ?
  App_Fatal("Not enough space for another mesh, fatal.\n") : "Ok.";
  (renderer->verts_count + mesh_verts_count > renderer->verts_cap) ?
  App_Fatal("Not enough space for mesh in verts_buffer, fatal.\n") : "Ok.";
  (renderer->inds_count + mesh_inds_count > renderer->inds_cap) ?
  App_Fatal("Not enough space for mesh in inds_buffer, fatal.\n") : "Ok.";

  glBindBuffer(GL_ARRAY_BUFFER, renderer->verts_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, renderer->verts_count * sizeof(Pipeline_Vertex), mesh_verts_count * sizeof(Pipeline_Vertex), verts);
  renderer->verts_count += mesh_verts_count;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->inds_buffer);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, renderer->inds_count * sizeof(GLuint), mesh_inds_count * sizeof(GLuint), indices);
  renderer->inds_count += mesh_inds_count;

  renderer->meshes_count += 1;
  renderer->mesh_ind_offset[renderer->meshes_count] = renderer->inds_count;
}
void Instance_Renderer_RenderMesh(Pipeline_Instance_Renderer* renderer, const GLuint mesh_index) {
  (mesh_index >= renderer->meshes_count) ?
  App_Fatal("Woops, cannot draw that mesh since it doesn't exist, fatal.\n") : "Ok.";
  #define Renderer_Mesh_Indices_Count(renderer, mesh_index) (renderer->mesh_ind_offset[mesh_index+1] - renderer->mesh_ind_offset[mesh_index])
  glBindVertexArray(renderer->vert_array);
  glDrawArraysInstanced(GL_TRIANGLES, renderer->mesh_ind_offset[mesh_index], Renderer_Mesh_Indices_Count(renderer, mesh_index), 1);
}
void Instance_Renderer_Destroy(Pipeline_Instance_Renderer** _renderer) {
  if(_renderer == NULL) return;
  Pipeline_Instance_Renderer* renderer = *_renderer;
  if(renderer != NULL) {
    free(renderer->mesh_ind_offset);
    glDeleteBuffers(1, &renderer->verts_buffer);
    glDeleteBuffers(1, &renderer->inds_buffer);
    glDeleteVertexArrays(1, &renderer->vert_array);
  }
  *_renderer = NULL;
}
