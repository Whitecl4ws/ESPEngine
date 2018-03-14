#ifndef RENDERER_INCLUDE_GUARD
#define RENDERER_INCLUDE_GUARD
#include "pipeline.h"
#include <graphene-1.0/graphene.h>
/* A bug with the library, you must copy graphere-config.h into graphene-1.0/  */
typedef struct {
  graphene_point3d_t position;
} __attribute__((packed)) Pipeline_Vertex;
typedef struct {
  GLuint verts_buffer, inds_buffer, vert_array;
  GLuint verts_count, verts_cap, inds_count, inds_cap, *mesh_ind_offset, meshes_count, meshes_cap;
} Pipeline_Instance_Renderer;
void App_Fatal(const char* fmt, ...);
Pipeline_Instance_Renderer* Pipeline_CreateInstanceRenderer(const Pipeline_Program program,
const GLuint meshes_cap, const GLuint verts_cap, const GLuint inds_cap);
void Instance_Renderer_CreateMesh(Pipeline_Instance_Renderer* renderer, const Pipeline_Vertex* vertices,
const GLuint* indices, const GLuint mesh_verts_count, const GLuint mesh_inds_count);
void Instance_Renderer_RenderMesh(Pipeline_Instance_Renderer* renderer, const GLuint mesh_index);
void Instance_Renderer_Destroy(Pipeline_Instance_Renderer** renderer);
#endif
