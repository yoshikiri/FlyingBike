#include "Result.h"

#include "fstream"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <irrKlang/irrKlang.h>
#include <map>
#include <stb_image.h>
#include <string>

#include "Camera.h"
#include "DrawFigure.h"
#include "Figure.h"
#include "Play.h"
#include "Shader.h"
#include "Title.h"

struct Character {
  GLuint textureID;
  glm::ivec2 size;
  glm::ivec2 bearing;
  GLuint advance;
};

namespace {
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

std::map<GLchar, Character> characters;

auto soundEngine = std::make_unique<irrklang::ISoundEngine *>(
    irrklang::createIrrKlangDevice());

std::string highScoreFile[] = {
    "resource/data/HighScore0.txt", "resource/data/HighScore1.txt",
    "resource/data/HighScore2.txt", "resource/data/HighScore3.txt"};

const glm::vec3 cameraPositionLookDown(0.0f, 0.0f, 10.0f);
const glm::vec3 cameraEye(0.0f, 0.0f, 0.0f);
const glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;
} // namespace

Result::Result(GLFWwindow *window, float score, bool isClear,
               unsigned int stage)
    : State(window),
      lightShader(std::make_unique<Shader>("resource/shader/light.vert",
                                           "resource/shader/light.frag")),
      objectShader(
          std::make_unique<Shader>("resource/shader/directionalLight.vert",
                                   "resource/shader/directionalLight.frag")),
      textShader(std::make_unique<Shader>("resource/shader/text.vert",
                                          "resource/shader/text.frag")),
      camera(std::make_unique<Camera>(cameraPositionLookDown, cameraEye,
                                      cameraUp, WIDTH, HEIGHT, true)),
      numberTextures(std::make_unique<unsigned int[]>(11)),
      resultTextures(std::make_unique<unsigned int[]>(4)), score(score),
      highScores(std::make_unique<float[]>(5)), isClear(isClear), stage(stage),
      updateHighScore(false) {

  if (isClear)
    (*soundEngine)
        ->play2D("resource/sound/se_maoudamashii_jingle05.ogg", false);
  else
    (*soundEngine)
        ->play2D("resource/sound/se_maoudamashii_jingle02.ogg", false);

  for (int i = 0; i < 10; i++) {
    char buff[5];
    snprintf(buff, sizeof(buff), "%d", i);
    numberTextures[i] = loadTexture(std::string("resource/image/number_result/") +
                                    std::string(buff) + ".png");
  }
  numberTextures[10] = loadTexture("resource/image/number_result/period.png");

  resultTextures[0] = loadTexture("resource/image/clear.png");
  resultTextures[1] = loadTexture("resource/image/failure.png");
  resultTextures[2] = loadTexture("resource/image/new.png");
  resultTextures[3] = loadTexture("resource/image/guide_result.png");

  initShaders();

  loadHighScore(highScoreFile[stage]);
  std::vector<float> v;
  if (isClear)
    v.push_back(score);
  for (int i = 0; i < 5; i++) {
    v.push_back(highScores[i]);
  }

  std::sort(v.begin(), v.end());

  // new record
  if (isClear) {
    if (v[4] != score)
      updateHighScore = true;

    v.erase(v.begin() + 5);
    for (int i = 0; i < 5; i++) {
      highScores[i] = v[i];
      // std::cout << highScores[i] << '\n';
    }
  }

  writeHighScore(highScoreFile[stage]);
}

State *Result::update() {
  State *next = this;

  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
    next = new Title(window);
  }
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
    std::cout << isClear << '\n';
    std::cout << score << '\n';
    next = new Play(window, stage);
  }

  draw();
  updateShaders();

  return next;
}

void Result::draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glActiveTexture(GL_TEXTURE0);
  if (isClear) {
    glBindTexture(GL_TEXTURE_2D, resultTextures[0]);
    DrawFigure::drawPlane(glm::vec3(3, -3.5, 0), glm::vec2(12, 4));
  } else {
    glBindTexture(GL_TEXTURE_2D, resultTextures[1]);
    DrawFigure::drawPlane(glm::vec3(2, -3.5, 0), glm::vec2(12, 4));
  }

  drawScore(glm::vec2(6, -1), score);

  for (int i = 0; i < 5; i++) {
    drawScore(glm::vec2(-2, -1 + i), highScores[i]);
    if (updateHighScore && highScores[i] == score) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, resultTextures[2]);
      DrawFigure::drawPlane(glm::vec3(0, -1 + i, 0),
                            glm::vec2(3, 3.39f / 3.0f));
    }
  }

  // draw guide
  glBindTexture(GL_TEXTURE_2D, resultTextures[3]);
  DrawFigure::drawPlane(glm::vec3(1, 4.5, 0), glm::vec2(12, 1));
}

void Result::initShaders() {
  objectShader->use();

  objectShader->use();
  objectShader->setMat4("model", glm::mat4(1.0f));
  objectShader->setMat4("view", camera->getView());
  objectShader->setMat4("projection", camera->getProjection());

  objectShader->setInt("material.diffuse", 0);
  objectShader->setInt("material.specular", 1);

  objectShader->setVec3("viewPos", camera->getPosition());
  objectShader->setFloat("material.shininess", 32.0f);

  objectShader->setVec3("dirLight.direction", 0.0f, 0.0f, 0.3f);
  objectShader->setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
  objectShader->setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
  objectShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
}

void Result::updateShaders() {
  objectShader->use();
  objectShader->setMat4("model", glm::mat4(1.0f));
  objectShader->setMat4("view", camera->getView());
  objectShader->setMat4("projection", camera->getProjection());
}

void Result::drawScore(glm::vec2 pos, float score) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);

  glm::mat4 model(glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0)));
  model = glm::rotate(model, (float)M_PI, glm::vec3(0, 1, 0));
  objectShader->setMat4("model", model);

  char buff[20];
  snprintf(buff, sizeof(buff), "%.3f", score);
  std::string scoreString(buff);
  int periodIndex = scoreString.find(".");

  float scoreX = 0;
  for (int i = 0; i < scoreString.length(); i++) {
    if (i == periodIndex) {
      scoreX -= 0.3;
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, numberTextures[10]);
      DrawFigure::drawPlane(glm::vec3(scoreX, 0.22, -0.1), glm::vec2(0.1, 0.1));
      scoreX += 0.4;
    } else {
      glActiveTexture(GL_TEXTURE0);
      char num = scoreString[i];
      glBindTexture(GL_TEXTURE_2D, numberTextures[num - '0']);
      DrawFigure::drawPlane(glm::vec3(scoreX, 0, 0), glm::vec2(1, 1));
      scoreX += 0.7;
    }
  }

  objectShader->setMat4("model", glm::mat4(1.0f));
}

void Result::loadHighScore(std::string filepath) {
  std::ifstream ifs(filepath);
  std::string str;

  if (ifs.fail()) {
    std::cerr << "failed to open " << filepath << '\n';
    return;
  }
  int i = 0;
  while (getline(ifs, str)) {
    char buff[10];
    snprintf(buff, sizeof(buff), "%.3f", std::stof(str));
    highScores[i] = std::stof(std::string(buff));
    i++;
  }
}

void Result::writeHighScore(std::string filepath) {
  std::ofstream ofs(filepath);
  for (int i = 0; i < 5; i++) {
    ofs << highScores[i] << '\n';
  }
}
