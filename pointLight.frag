#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

in vec3 FragmentPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragment;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {

  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragmentPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse =
      light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

  // specular
  vec3 viewDir = normalize(viewPos - FragmentPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular =
      light.specular * spec * vec3(texture(material.specular, TexCoord));

  // attenaution
  float distance = length(light.position - FragmentPos);


  float attenaution = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
  // float attenaution = 1.0 / (1.0 + 0.09f * distance +
  //                         0.032f * (distance * distance));

  ambient *= attenaution;
  diffuse *= attenaution;
  specular *= attenaution;

  vec3 result = ambient + diffuse + specular;
  fragment = vec4(result, 1.0);
  //fragment = vec4(vec3(attenaution), 1.0);
}