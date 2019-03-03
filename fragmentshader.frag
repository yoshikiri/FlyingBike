#version 330 core

struct Material{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

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
uniform Light light;
//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
  //fragment = texture(texture_diffuse1, TexCoord);

  vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragmentPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

  vec3 viewDir = normalize(viewPos - FragmentPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

  vec3 result = ambient + diffuse + specular;
  fragment = vec4(result, 1.0);
}