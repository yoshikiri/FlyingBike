#version 330 core

//in vec4 vertex_color;
in vec2 TexCoord;

out vec4 fragment;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main(){
  //fragment = vertex_color;
  fragment = texture(texture_diffuse1, TexCoord);
}