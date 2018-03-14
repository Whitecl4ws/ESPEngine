#include <stdio.h>
#include <stdlib.h>
#include "includes/camera.h"
#include <GLFW/glfw3.h>
struct {
  char WindowFocused;
} App_Toggles = {0};
extern Pipeline_Camera* camera;
void Callback_Fatal(const int code, const char* message) {
  printf("%s\n", message);
  exit(code);
}
void Callback_Mouse_Move(GLFWwindow* window, double x, double y) {
  if(App_Toggles.WindowFocused) {
    camera->mouse_position.x += 0.02 * (x - camera->previous_position.x);
    camera->mouse_position.y += 0.02 * (y - camera->previous_position.y);
    kmVec2Fill(&camera->previous_position, x, y);
    Camera_Update_Rotation(camera);
  }
}
void Keyboard_KeyDownListener(GLFWwindow* window) {
  #define IsKeyDown(key) (glfwGetKey(window, key) == GLFW_PRESS)
  if(IsKeyDown(GLFW_KEY_UP)) {
    Camera_Move_Forward(camera, 0.05);
  } else if(IsKeyDown(GLFW_KEY_DOWN)) {
    Camera_Move_Forward(camera, -0.05);
  }
  if(IsKeyDown(GLFW_KEY_LEFT)) {
    Camera_Move_Right(camera, -0.05);
  } else if(IsKeyDown(GLFW_KEY_RIGHT)) {
    Camera_Move_Right(camera, 0.05);
  }
}
void Callback_Keyboard_PressOnce(GLFWwindow* window, const int key,
  const int scancode, const int action, const int mods) {
  if(action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_ESCAPE:
        App_Toggles.WindowFocused = !App_Toggles.WindowFocused;
        if(App_Toggles.WindowFocused) {
          double position[2];
          glfwGetCursorPos(window, &position[0], &position[1]);
          kmVec2Fill(&camera->previous_position, position[0], position[1]);
          glfwSetCursorPosCallback(window, Callback_Mouse_Move);
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          /* ToDo: Fix the mouse position on focus out! */
        } else {
          glfwSetCursorPosCallback(window, NULL);
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        break;
    }
  }
}
