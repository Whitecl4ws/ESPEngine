#ifndef CAMERA_HEADER_GUARD
#define CAMERA_HEADER_GUARD
#include "pipeline.h"
#include <kazmath/kazmath.h>
/* compile kazmath! */
typedef struct {
  kmVec3 position, direction, axis;
  kmMat4 rotation;
  kmVec2 mouse_position, previous_position;
  Pipeline_Uniform view_matrix_location;
} Pipeline_Camera;
Pipeline_Camera* Pipeline_CreateCamera();
void Camera_Move_Forward(Pipeline_Camera* camera, const float delta);
void Camera_Move_Right(Pipeline_Camera* camera, const float delta);
void Camera_Update_Rotation(Pipeline_Camera* camera);
void Camera_Update(Pipeline_Camera* camera); /* Don't call this! */
void Camera_Destroy(Pipeline_Camera** camera);
#endif
