#include <stdio.h>
#include <stdlib.h>
#include "../vendor/glad.h"
#include <GLFW/glfw3.h>
#include "includes/renderer.h"
#include "includes/camera.h"
#include "includes/mesh.h"
void App_Fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  exit(0);
}
/* Implement TextureManager! */
/* export LD_LIBRARY_PATH=/usr/local/lib/x86_64-linux-gnu/ (GRAPHENE, change it for cross-compile) */
Pipeline_Camera* camera = NULL;
void Callback_Fatal(const int code, const char* message);
void Keyboard_KeyDownListener(GLFWwindow* window);
void Callback_Keyboard_PressOnce(GLFWwindow* window, const int key,
  const int scancode, const int action, const int mods);
int main(const int argc, char const *argv[]) {
  glfwSetErrorCallback(Callback_Fatal);
  glfwInit();
  atexit(glfwTerminate);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, 1);

  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello, World!", NULL, NULL);
  glfwMakeContextCurrent(window);
  (!gladLoadGL()) ?
  App_Fatal("Failed to load OpenGL, fatal.\n"): "Success, OpenGL loaded.";

  Pipeline_Program program[2];
  program[0] = Pipeline_CreateProgram(2, "assets/shaders/basic.vert", GL_VERTEX_SHADER,
  "assets/shaders/basic.frag", GL_FRAGMENT_SHADER);
  program[1] = Pipeline_CreateProgram(2, "assets/shaders/skybox.vert", GL_VERTEX_SHADER,
  "assets/shaders/skybox.frag", GL_FRAGMENT_SHADER);

  kmMat4 projection;
  glUseProgram(program[0]);
  Pipeline_Uniform uniform = Pipeline_GetUniform(program[0], "projection");
  kmMat4PerspectiveProjection(&projection, 55, 640.0 / 480, 0.1, 100);
  glUniformMatrix4fv(uniform, 1, GL_FALSE, projection.mat);
  const Pipeline_Uniform uniforms[] =  {
    /* Pipeline_GetUniform(program[0], "time") */
  };

  Pipeline_Instance_Renderer* renderer = Meshes_CreateInstanceRenderer(program[0], 1, "assets/meshes/triangle.mesh");
  camera = Pipeline_CreateCamera(program[0]);
  Camera_Update(camera);
  glfwSetKeyCallback(window, Callback_Keyboard_PressOnce);

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    Keyboard_KeyDownListener(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program[0]);
    glUniform1f(uniforms[0], glfwGetTime());
    Instance_Renderer_RenderMesh(renderer, 0);
    glfwSwapBuffers(window);
  }
  Instance_Renderer_Destroy(&renderer);
  Camera_Destroy(&camera);
}
