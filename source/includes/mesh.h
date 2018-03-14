#ifndef MESH_INCLUDE_GUARD
#define MESH_INCLUDE_GUARD
#include "renderer.h"
typedef struct {
  Pipeline_Vertex* vertices;
  GLuint verts_count, *indices, inds_count;
} Mesh_Singleton;
Pipeline_Instance_Renderer* Meshes_CreateInstanceRenderer
 (const Pipeline_Program program, const GLuint mesh_count, ...);
Mesh_Singleton* Mesh_Parse(const char* path);
void Mesh_Free(Mesh_Singleton** mesh);
#endif
