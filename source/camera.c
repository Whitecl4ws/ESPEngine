#include <stdio.h>
#include <stdlib.h>
#include "includes/camera.h"
Pipeline_Camera* Pipeline_CreateCamera(Pipeline_Program program) {
  Pipeline_Camera* camera = calloc(1, sizeof(Pipeline_Camera));
  camera->view_matrix_location = Pipeline_GetUniform(program, "viewmatrix");
  kmMat4Identity(&camera->rotation);
  camera->direction.x = 1;
  return camera;
}
void Camera_Move_Forward(Pipeline_Camera* camera, const float delta) {
  kmVec3 offset;
  kmVec3Scale(&offset, &camera->direction, delta);
  kmVec3Add(&camera->position, &camera->position, &offset);
  Camera_Update(camera);
}
void Camera_Move_Right(Pipeline_Camera* camera, const float delta) {
  kmVec3 offset;
  kmVec3Scale(&offset, &camera->axis, delta);
  kmVec3Add(&camera->position, &camera->position, &offset);
  Camera_Update(camera);
}
const kmVec3 UP = {0, 1, 0};
void Camera_Update_Rotation(Pipeline_Camera* camera) {
  #define degsToRads(rads) ((rads) * M_PI / 180.0)
  const float phi = camera->mouse_position.x;
  const float theta = (camera->mouse_position.y >= degsToRads(85)) ? -degsToRads(85) :
  ((camera->mouse_position.y <= -degsToRads(85)) ? degsToRads(85) : -camera->mouse_position.y);
  camera->direction.y = sin(theta);
  camera->direction.x = cos(theta) * cos(phi);
  camera->direction.z = cos(theta) * sin(phi);
  kmVec3 zero = {0};
  kmMat4LookAt(&camera->rotation, &zero, &camera->direction, &UP);
  kmVec3Cross(&camera->axis, &camera->direction, &UP);
  Camera_Update(camera);
}
/* This looks heavy, but if it works, we can optimise later! */
void Camera_Update(Pipeline_Camera* camera) {
  kmMat4 view;
  kmVec3 center;
  kmVec3Add(&center, &camera->position, &camera->direction);
  kmMat4LookAt(&view, &camera->position, &center, &UP);
  glUniformMatrix4fv(camera->view_matrix_location, 1, GL_FALSE, view.mat);
} /* Don't call this! */
void Camera_Destroy(Pipeline_Camera** _camera) {
  if(_camera == NULL) return;
  Pipeline_Camera* camera = *_camera;
  if(camera != NULL) {
    free(camera);
  }
  *_camera = NULL;
}
