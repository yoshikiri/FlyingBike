#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define NUM_POINT_LIGHTS 4
#define NUM_SPOT_LIGHTS 4

in vec3 FragmentPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 fragment;

uniform Material material;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform SpotLight spotLight;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
  // properties
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragmentPos);

  // phase 1: Directional lighting
  vec3 result = calcDirLight(dirLight, norm, viewDir);
  // phase 2: Point lights
  for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    result += calcPointLight(pointLights[i], norm, FragmentPos, viewDir);

  // phase 3: Spot light
  for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    result += CalcSpotLight(spotLights[i], norm, FragmentPos, viewDir);

  fragment = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

  // difuse
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse =
      light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular =
      light.specular * spec * vec3(texture(material.specular, TexCoord));

  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
  // combine results
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
  vec3 diffuse =
      light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
  vec3 specular =
      light.specular * spec * vec3(texture(material.specular, TexCoord));
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
  // spotlight intensity
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  // combine results
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
  vec3 diffuse =
      light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
  vec3 specular =
      light.specular * spec * vec3(texture(material.specular, TexCoord));

  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return (ambient + diffuse + specular);
}