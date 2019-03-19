#include "Title.h"

#include <glm/gtc/matrix_transform.hpp>
#include <irrKlang/irrKlang.h>
#include <stb_image.h>

#include "Camera.h"
#include "DrawFigure.h"
#include "Play.h"
#include "Shader.h"

namespace {
auto soundEngine = std::make_unique<irrklang::ISoundEngine *>(
    irrklang::createIrrKlangDevice());

const glm::vec3 cameraPositionLookDown(0.0f, 0.0f, 10.0f);
const glm::vec3 cameraEye(0.0f, 0.0f, 0.0f);
const glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

float curTime;
float preTime;
float curTime2;
float preTime2;
const float INTERVAL = 0.2f;

const int STAGE_NUM = 4;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
  }
}

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
      lightShader(std::make_unique<Shader>("resource/shader/light.vert",
                                           "resource/shader/light.frag")),
      objectShader(
          std::make_unique<Shader>("resource/shader/directionalLight.vert",
                                   "resource/shader/directionalLight.frag")),
      camera(std::make_unique<Camera>(cameraPositionLookDown, cameraEye,
                                      cameraUp, WIDTH, HEIGHT, true)),
      numberTextures(std::make_unique<unsigned int[]>(10)),
      titleTextures(std::make_unique<unsigned int[]>(3)), stage(0) {

  for (int i = 0; i < 10; i++) {
    char buff[5];
    snprintf(buff, sizeof(buff), "%d", i);
    numberTextures[i] = loadTexture(std::string("resource/image/number_result/") +
                                    std::string(buff) + ".png");
  }

  titleTextures[0] = loadTexture("resource/image/title.png");
  titleTextures[1] = loadTexture("resource/image/guide_title.png");
  titleTextures[2] = loadTexture("resource/image/stage.png");

  initShaders();
  glfwSetKeyCallback(window, key_callback);

  curTime = glfwGetTime();
  preTime = -1;
  curTime2 = glfwGetTime();
  preTime2 = -1;
}

State *Title::update() {
  State *next = this;

  // start game
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
    (*soundEngine)
        ->play2D("resource/sound/se_start_play.ogg", false);
    next = new Play(window, stage);

    // stage select
  } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    curTime = glfwGetTime();
    if (curTime > preTime + INTERVAL) {
      (*soundEngine)
          ->play2D("resource/sound/se_select_stage.ogg", false);
      stage = (stage + 1) % STAGE_NUM;
      preTime = curTime;
    }
  } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    curTime2 = glfwGetTime();
    if (curTime2 > preTime2 + INTERVAL) {
      (*soundEngine)
          ->play2D("resource/sound/se_select_stage.ogg", false);
      stage = (stage + (STAGE_NUM - 1)) % STAGE_NUM;
      preTime2 = curTime2;
    }
  }

  updateShaders();
  draw();

  return next;
}

void Title::draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);

  // draw title
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, titleTextures[0]);
  DrawFigure::drawPlane(glm::vec3(0.5, -4, 0), glm::vec2(15, 3));

  // draw key guide
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, titleTextures[1]);
  DrawFigure::drawPlane(glm::vec3(1.2, 3.5, 0), glm::vec2(11, 2));

  // draw stage
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, titleTextures[2]);
  DrawFigure::drawPlane(glm::vec3(4.5, 0, 0), glm::vec2(6, 2));

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
