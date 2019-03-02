#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertex_color;
out vec2 TexCoord;

void main() {
  //vertex_color = color;
  TexCoord = inTexCoord;
  gl_Position = projection * view * model * vec4(position, 1.0);
}