#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 viewmatrix;
uniform float time;
void main()
{
  gl_Position = projection * viewmatrix * vec4(position, 1.0);
}
