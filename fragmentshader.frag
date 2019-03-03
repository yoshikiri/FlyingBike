#version 330 core

struct Material{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
}

//in vec4 vertex_color;
in vec3 FragmentPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragment;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform Material material;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
  //fragment = vertex_color;
  //fragment = texture(texture_diffuse1, TexCoord);
  fragment = vec4(lightColor * objectColor, 1.0);

  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragmentPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragmentPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  fragment = vec4(result, 1.0);
}