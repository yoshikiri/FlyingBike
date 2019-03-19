#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragmentPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
  FragmentPos = vec3(model * vec4(position, 1.0));
  TexCoord = texCoord;
  Normal = mat3(transpose(inverse(model))) * normal;
  
  gl_Position = projection * view * model * vec4(position, 1.0);
}