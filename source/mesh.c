#include <stdio.h>
#include "includes/mesh.h"
#include "includes/files.h"
/* Correct format:
vertices %a
%f %f %f
%f %f %f
%f %f %f
... %a times!
indices %b
%ud
%ud
%ud
... %b times! */
Mesh_Singleton* Mesh_Parse(const char* path) {
  FILE* file = File_Open(path, "r");
  Mesh_Singleton* mesh = calloc(1, sizeof(Mesh_Singleton));

  (fscanf(file, "vertices %d\n", &mesh->verts_count) != 1) ?
  App_Fatal("Mesh at %s, incorrect format, fatal.\n", path) : "Ok.";
  mesh->vertices = calloc(mesh->verts_count, sizeof(Pipeline_Vertex));
  for(int i = 0; i < mesh->verts_count; ++i) {
    (fscanf(file, "%f %f %f\n", &mesh->vertices[i].position.x,
    &mesh->vertices[i].position.y, &mesh->vertices[i].position.z) != 3) ?
    App_Fatal("Mesh at %s, incorrect format, fatal.\n", path) : "Ok.";
  }

  (fscanf(file, "indices %d\n", &mesh->inds_count) != 1) ?
  App_Fatal("Mesh at %s, incorrect format, fatal.\n", path) : "Ok.";
  mesh->indices = calloc(mesh->inds_count, sizeof(GLuint));
  for(int i = 0; i < mesh->inds_count; ++i) {
    (fscanf(file, "%ud", &mesh->indices[i]) != 1) ?
    App_Fatal("Mesh at %s, incorrect format, fatal.\n", path) : "Ok.";
  }

  fclose(file);
  return mesh;
}
void Mesh_Free(Mesh_Singleton** _mesh) {
  if(_mesh == NULL) return;
  Mesh_Singleton* mesh = *_mesh;
  if(mesh != NULL) {
    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
  }
  *_mesh = NULL;
}
Pipeline_Instance_Renderer* Meshes_CreateInstanceRenderer(const Pipeline_Program program, const GLuint meshes_count, ...) {
  va_list args;
  va_start(args, meshes_count);
  GLuint total_inds = 0, total_verts = 0;

  Mesh_Singleton** meshes = calloc(meshes_count, sizeof(Mesh_Singleton*));
  for(int i = 0; i < meshes_count; ++i) {
    meshes[i] = Mesh_Parse(va_arg(args, char*));
    total_verts += meshes[i]->verts_count;
    total_inds += meshes[i]->inds_count;
  }

  Pipeline_Instance_Renderer* renderer = Pipeline_CreateInstanceRenderer(program, meshes_count, total_verts, total_inds);
  for(int i = 0; i < meshes_count; ++i) {
    Instance_Renderer_CreateMesh(renderer, meshes[i]->vertices, meshes[i]->indices, meshes[i]->verts_count, meshes[i]->inds_count);
    Mesh_Free(&meshes[i]);
  }

  free(meshes);
  va_end(args);
  return renderer;
}
