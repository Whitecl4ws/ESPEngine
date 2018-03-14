#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include "includes/pipeline.h"
#include "includes/files.h"
Pipeline_Shader Pipeline_CreateShader(const char* path, const GLenum type) {
  Pipeline_Shader shader = glCreateShader(type);
  const char* source = File_Load(path);
  glShaderSource(shader, 1, &source, NULL);
  free((char*)source);
  glCompileShader(shader);

  char infolog[1024],* _path = (char*) path;
  glGetShaderInfoLog(shader, 1024, NULL, infolog);
  (infolog[0] != '\0') ?
  App_Fatal("%s at %s", basename(_path), infolog) : "Success, shader compiled.";
  return shader;
}
/* Expected variadic pattern -> string - GLenum - string - GLenum ... */
Pipeline_Program Pipeline_CreateProgram(const int shader_count, ...) {
  va_list args;
  va_start(args, shader_count);
  Pipeline_Program program = glCreateProgram();
  Pipeline_Shader *shaders = calloc(shader_count, sizeof(Pipeline_Shader));

  for(int i = 0; i < shader_count; ++i) {
    shaders[i] = Pipeline_CreateShader(va_arg(args, char*), va_arg(args, GLenum));
    glAttachShader(program, shaders[i]);
  }
  glLinkProgram(program);

  for(int i = 0; i < shader_count; ++i) {
    glDetachShader(program, shaders[i]);
    glDeleteShader(shaders[i]);
  }
  va_end(args);
  free(shaders);
  return program;
}
Pipeline_Uniform Pipeline_GetUniform(const Pipeline_Program program, const char* name) {
  Pipeline_Uniform uniform = glGetUniformLocation(program, name);
  (uniform == -1) ?
  App_Fatal("Uniform %s does not exist/is unused, fatal.\n", name) : "Ok.";
  return uniform;
}
void Pipeline_CheckAttributeLocation(const Pipeline_Program program, const char* name, const GLint specified_location) {
  GLint attribute_location = glGetAttribLocation(program, name);
  (attribute_location != specified_location) ?
  App_Fatal("Attribute %s is at location=%d instead of location=%d, fatal.\n", name, attribute_location, specified_location) : "Ok.";
}
