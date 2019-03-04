#version 330 core

out vec4 Fragment;

in vec3 TexCoord;

uniform samplerCube skybox;

void main() {
  Fragment = texture(skybox, TexCoord);
}