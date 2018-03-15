#ifndef PIPELINE_INCLUDE_GUARD
#define PIPELINE_INCLUDE_GUARD
#include "../../vendor/glad.h"
typedef GLuint Pipeline_Shader;
Pipeline_Shader Pipeline_CreateShader(const char* path, const GLenum type);
typedef GLuint Pipeline_Program;
Pipeline_Program Pipeline_CreateProgram(const int shader_count, ...);
typedef GLint Pipeline_Uniform;
Pipeline_Uniform Pipeline_GetUniform(const Pipeline_Program program, const char* name);
void Pipeline_CheckAttributeLocation(const Pipeline_Program program, const char* name, const GLint specified_location);
/* Make sure to use a program before updating the camera! */
typedef GLuint Pipeline_Texture_Unit;
Pipeline_Texture_Unit Pipeline_NewTextureUnit();
#endif
