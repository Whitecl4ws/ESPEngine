#ifndef SKYBOX_INCLUDE_GUARD
#define SKYBOX_INCLUDE_GUARD
#include "pipeline.h"
#include "camera.h"
#include "files.h"
typedef struct {
  Files_Texture faces[6];
  Pipeline_Program program;
  GLuint texture, vert_array, mesh;
  Pipeline_Uniform view_matrix_location;
  Pipeline_Texture_Unit texture_unit;
} Pipeline_Skybox;
Pipeline_Skybox* Skybox_Create(const Pipeline_Program program, const char* path);
void Skybox_Render(const Pipeline_Skybox* skybox,
  const Pipeline_Camera* camera, const Pipeline_Program program);
void Skybox_Destroy(Pipeline_Skybox** skybox);
#endif
