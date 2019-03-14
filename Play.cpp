#include "Play.h"

// #include <glm/glm.hpp>
#include <cstdio>
#include <vector>

#include "Camera.h"
#include "Container.h"
#include "DrawFigure.h"
#include "Goal.h"
#include "Player.h"
#include "Result.h"
#include "Shader.h"

namespace {
const glm::vec3 containerPositions[] = {
    glm::vec3(0.0f, 0.0f, 0.5f),
    glm::vec3(2.5f, 1.5f, 0.5f),
    glm::vec3(-1.0f, 1.0f, 0.5f),
    glm::vec3(-2.0f, -2.0f, 0.5f),
};

const glm::vec3 goalPositions[] = {
    glm::vec3(-1.0f, 0.0f, 0.5f),
    glm::vec3(2.0f, 1.5f, 0.5f),
    glm::vec3(-3.0f, 1.0f, 0.5f),
    glm::vec3(-2.0f, -3.0f, 0.5f),
};

const glm::vec3 cameraPositionLookDown(0.0f, 0.0f, 10.0f);
const glm::vec3 cameraPositionPlay(0.0f, 5.0f, 5.0f);
const glm::vec3 cameraEye(0.0f, 0.0f, 0.0f);
const glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

const int NUM_SPOT_LIGHTS = 4;

} // namespace

Play::Play(GLFWwindow *window, glm::vec3 center, unsigned int textures[])
    : State(window),
      player(std::make_unique<Player>(center, textures[0], textures[1])),
      containers(std::make_unique<
                 std::vector<Container, std::allocator<Container>>>()),
      goals(std::make_unique<std::vector<Goal, std::allocator<Goal>>>()),
      lightShader(std::make_unique<Shader>("resource/shader/light.vert",
                                           "resource/shader/light.frag")),
      objectShader(std::make_unique<Shader>(
          "resource/shader/directionalAndSpotLightObject.vert",
          "resource/shader/directionalAndSpotLightObject.frag")),
      skyboxShader(std::make_unique<Shader>("resource/shader/skybox.vert",
                                            "resource/shader/skybox.frag")),
      camera(std::make_unique<Camera>(cameraPositionPlay, cameraEye,
                                      cameraUp, WIDTH, HEIGHT, true)),
      textures(new unsigned int[6]) {

  //--------------------------------------------------------------------------//
  for (int i = 0; i < 6; i++)
    this->textures[i] = textures[i];

  for (glm::vec3 p : containerPositions)
    containers->emplace_back(p, glm::vec3(1.0f), textures[2], textures[3]);
  for (glm::vec3 p : goalPositions)
    goals->emplace_back(p, 0.45f);

  initShaders();
}

State *Play::update() {
  State *next = this;

  updateShaders();

  if (checkClear())
    next = new Result(window);

  draw();
  // next = new Result(window);
  return next;
}

void Play::draw() {
  // draw containers
  for (Container c : *containers) {
    c.draw();
  }

  // draw planes
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 10; x++) {
      DrawFigure::drawPlane(glm::vec3(-4 + x, -4 + y, 0), glm::vec2(1, 1));
    }
  }

  // draw player
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    player->velocity.x += 0.001;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    player->velocity.x -= 0.001;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    player->velocity.y += 0.001;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    player->velocity.y -= 0.001;

  // player->seek(target);

  // check if player's collision
  for (Container c : *containers)
    player->checkCollision(c);

  for (Goal &g : *goals)
    player->checkGoal(g);

  player->update();
  player->draw();
}

bool Play::checkClear() {
  for (Goal g : *goals) {
    if (!g.isClear)
      return false;
  }
  return true;
}

void Play::initShaders() {
  objectShader->use();

  objectShader->use();
  objectShader->setMat4("model", glm::mat4(1.0f));
  objectShader->setMat4("view", camera->getView());
  objectShader->setMat4("projection", camera->getProjection());

  objectShader->setInt("material.diffuse", 0);
  objectShader->setInt("material.specular", 1);

  objectShader->setVec3("viewPos", camera->getPosition());
  objectShader->setFloat("material.shininess", 32.0f);

  // directional light
  objectShader->setVec3("dirLight.direction", 0.2f, 0.2f, -0.3f);
  objectShader->setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
  objectShader->setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
  objectShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

  // spotLights
  for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {
    char buff[30];
    snprintf(buff, sizeof(buff), "spotLights[%d].position", i);
    objectShader->setVec3(std::string(buff),
                          goalPositions[i] + glm::vec3(0.0f, 0.0f, 1.5f));
    snprintf(buff, sizeof(buff), "spotLights[%d].direction", i);
    objectShader->setVec3(std::string(buff), glm::vec3(0.0f, 0.0f, -1.0f));
    snprintf(buff, sizeof(buff), "spotLights[%d].ambient", i);
    objectShader->setVec3(std::string(buff), (*goals)[i].color);
    snprintf(buff, sizeof(buff), "spotLights[%d].diffuse", i);
    objectShader->setVec3(std::string(buff), 1.0f, 1.0f, 1.0f);
    snprintf(buff, sizeof(buff), "spotLights[%d].specular", i);
    objectShader->setVec3(std::string(buff), 1.0f, 1.0f, 1.0f);
    snprintf(buff, sizeof(buff), "spotLights[%d].constant", i);
    objectShader->setFloat(std::string(buff), 1.0f);
    snprintf(buff, sizeof(buff), "spotLights[%d].linear", i);
    objectShader->setFloat(std::string(buff), 0.09f);
    snprintf(buff, sizeof(buff), "spotLights[%d].quadratic", i);
    objectShader->setFloat(std::string(buff), 0.032f);
    snprintf(buff, sizeof(buff), "spotLights[%d].cutOff", i);
    objectShader->setFloat(std::string(buff), glm::cos(glm::radians(12.5f)));
    snprintf(buff, sizeof(buff), "spotLights[%d].outerCutOff", i);
    objectShader->setFloat(std::string(buff), glm::cos(glm::radians(15.0f)));
  }
}

void Play::updateShaders() {
  objectShader->use();
  objectShader->setMat4("model", glm::mat4(1.0f));
  objectShader->setMat4("view", camera->getView());
  objectShader->setMat4("projection", camera->getProjection());

  for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {
    char buff[30];
    snprintf(buff, sizeof(buff), "spotLights[%d].ambient", i);
    objectShader->setVec3(std::string(buff), (*goals)[i].color);
  }
}
