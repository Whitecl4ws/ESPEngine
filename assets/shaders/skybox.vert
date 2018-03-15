#version 330 core
in vec3 position;
uniform mat4 projection, viewmatrix;
out vec3 texcoords;
void main() {
  texcoords = position;
  gl_Position = projection * viewmatrix * vec4(position, 1.0);
}
