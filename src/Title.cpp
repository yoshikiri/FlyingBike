#include "Title.h"

#include <glm/gtc/matrix_transform.hpp>
#include <irrKlang/irrKlang.h>
#include <map>
#include <stb_image.h>

#include "Camera.h"
#include "DrawFigure.h"
#include "Play.h"
#include "Shader.h"

namespace {
const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;
const glm::vec3 cameraPositionLookDown(0.0f, 0.0f, 10.0f);
const glm::vec3 cameraEye(0.0f, 0.0f, 0.0f);
const glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

auto soundEngine = std::make_unique<irrklang::ISoundEngine *>(
    irrklang::createIrrKlangDevice());

const int STAGE_NUM = 4;

// key input interval
float curTime[2];
float preTime[2];
const float INTERVAL = 0.2f;

std::map<std::string, int> TEXTURE_INDEX = {
    {"title", 0}, {"stage", 1}, {"guide_title", 2}};

unsigned int loadTexture(std::string filename, bool useAlpha = true) {
  stbi_set_flip_vertically_on_load(false);

  int WIDTH, HEIGHT, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &WIDTH, &HEIGHT, &nrChannels, STBI_rgb_alpha);
  if (data == 0) {
    std::cout << "cannot load " << filename << '\n';
  }

  unsigned int textureID = 0;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (useAlpha == false)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  if (useAlpha == true)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return textureID;
}
} // namespace

Title::Title(GLFWwindow *window)
    : State(window),

      objectShader(
          std::make_unique<Shader>("resource/shader/directionalLight.vert",
                                   "resource/shader/directionalLight.frag")),
      camera(std::make_unique<Camera>(cameraPositionLookDown, cameraEye,
                                      cameraUp, WIDTH, HEIGHT, true)),
      titleTextures(std::make_unique<unsigned int[]>(TEXTURE_INDEX.size())),
      numberTextures(std::make_unique<unsigned int[]>(10)), stage(0) {

  // load textures
  for (int i = 0; i < 10; i++) {
    char buff[5];
    snprintf(buff, sizeof(buff), "%d", i);
    numberTextures[i] =
        loadTexture(std::string("resource/image/number_result/") +
                    std::string(buff) + ".png");
  }

  for (auto p : TEXTURE_INDEX)
    titleTextures[p.second] = loadTexture("resource/image/" + p.first + ".png");

  initShaders();

  for (int i = 0; i < 2; i++) {
    curTime[i] = glfwGetTime();
    preTime[i] = -INTERVAL;
  }
}

State *Title::update() {
  State *next = this;

  // start game
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
    (*soundEngine)->play2D("resource/sound/se_start_play.ogg", false);
    next = new Play(window, stage);

    // stage select
  } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    curTime[0] = glfwGetTime();
    if (curTime[0] > preTime[0] + INTERVAL) {
      (*soundEngine)->play2D("resource/sound/se_select_stage.ogg", false);
      stage = (stage + 1) % STAGE_NUM;
      preTime[0] = curTime[0];
    }
  } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    curTime[1] = glfwGetTime();
    if (curTime[1] > preTime[1] + INTERVAL) {
      (*soundEngine)->play2D("resource/sound/se_select_stage.ogg", false);
      stage = (stage + (STAGE_NUM - 1)) % STAGE_NUM;
      preTime[1] = curTime[1];
    }
  }

  updateShaders();
  draw();

  return next;
}

void Title::draw() const {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);

  // draw title
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, titleTextures[TEXTURE_INDEX["title"]]);
  DrawFigure::drawPlane(glm::vec3(0.5, -4, 0), glm::vec2(15, 3));

  // draw stage
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, titleTextures[TEXTURE_INDEX["stage"]]);
  DrawFigure::drawPlane(glm::vec3(4.5, 0, 0), glm::vec2(6, 2));

  // draw key guide
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, titleTextures[TEXTURE_INDEX["guide_title"]]);
  DrawFigure::drawPlane(glm::vec3(1.2, 3.5, 0), glm::vec2(11, 2));

  // draw stage number
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, numberTextures[stage]);
  glm::mat4 model =
      glm::rotate(glm::mat4(1.0f), (float)M_PI, glm::vec3(0, 1, 0));
  objectShader->setMat4("model", model);
  DrawFigure::drawPlane(glm::vec3(-1.0, -0.2, 0), glm::vec2(2, 2), true);
  objectShader->setMat4("model", glm::mat4(1.0f));
}

void Title::initShaders() {
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
  objectShader->setVec3("dirLight.direction", 0.0f, 0.0f, -0.3f);
  objectShader->setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
  objectShader->setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
  objectShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
}

void Title::updateShaders() {
  objectShader->use();
  objectShader->setMat4("model", glm::mat4(1.0f));
  objectShader->setMat4("view", camera->getView());
  objectShader->setMat4("projection", camera->getProjection());
}
