#include "Result.h"

#include "fstream"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <stb_image.h>
#include <string>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

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

std::string highScoreFile = "resource/HighScore.txt";

const glm::vec3 cameraPositionLookDown(0.0f, 0.0f, 10.0f);
// const glm::vec3 cameraPositionPlay(0.0f, 5.0f, 5.0f);
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

  for (int i = 0; i < 10; i++) {
    char buff[5];
    snprintf(buff, sizeof(buff), "%d", i);
    numberTextures[i] = loadTexture(std::string("resource/number_result/") +
                                    std::string(buff) + ".png");
  }
  numberTextures[10] = loadTexture("resource/number_result/period.png");

  resultTextures[0] = loadTexture("resource/clear.png");
  resultTextures[1] = loadTexture("resource/failure.png");
  resultTextures[2] = loadTexture("resource/new.png");
  resultTextures[3] = loadTexture("resource/guide_result.png");

  initShaders();

  loadHighScore(highScoreFile);
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
      std::cout << highScores[i] << '\n';
    }
  }

  writeHighScore(highScoreFile);

  // FT_Library ft;
  // if (FT_Init_FreeType(&ft))
  //   std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
  //             << std::endl;
  //
  // FT_Face face;
  // if (FT_New_Face(ft, "resource/fonts/FreeMono.ttf", 0, &face))
  //   std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
  //
  // FT_Set_Pixel_Sizes(face, 0, 48);
  // if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
  //   std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
  //
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment
  // restriction
  //
  // for (GLubyte c = 0; c < 128; c++) {
  //   // Load character glyph
  //   if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
  //     std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
  //     continue;
  //   }
  //   // Generate texture
  //   GLuint texture;
  //   glGenTextures(1, &texture);
  //   glBindTexture(GL_TEXTURE_2D, texture);
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
  //                face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
  //                face->glyph->bitmap.buffer);
  //   // Set texture options
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //   // Now store character for later use
  //   Character character = {
  //       texture,
  //       glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
  //       glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
  //       static_cast<GLuint>(face->glyph->advance.x)};
  //   characters.insert(std::pair<GLchar, Character>(c, character));
  // }
  //
  // FT_Done_Face(face);
  // FT_Done_FreeType(ft);
  //
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //
  // textShader->setMat4("projection", glm::ortho(-40.0f, 40.0f,
  // -30.0f, 30.0f));
}

State *Result::update() {
  State *next = this;

  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
    next = new Title(window);
  }
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
    std::cout << isClear << '\n';
    std::cout << score << '\n';
    next = new Play(window, glm::vec3(3, -0.5, 0.5), stage);
  }

  draw();
  updateShaders();

  return next;
}

void Result::draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glActiveTexture(GL_TEXTURE0);
  if (isClear){
    glBindTexture(GL_TEXTURE_2D, resultTextures[0]);
    DrawFigure::drawPlane(glm::vec3(3, -3.5, 0), glm::vec2(12, 4));
  }
  else{
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

  // directional light
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
  // objectShader->setMat4("model")
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

void Result::RenderText(Shader &s, std::string text, GLfloat x, GLfloat y,
                        GLfloat scale, glm::vec3 color) {
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Activate corresponding render state
  s.use();
  s.setVec3("textColor", color.x, color.y, color.z);
  // glUniform3f(
  //    glGetUniformLocation(s.program, "textColor"), color.x, color.y,
  //    color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = characters[*c];

    GLfloat xpos = x + ch.bearing.x * scale;
    GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

    GLfloat w = ch.size.x * scale;
    GLfloat h = ch.size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] = {
        {xpos, ypos + h, 0.0, 0.0},    {xpos, ypos, 0.0, 1.0},
        {xpos + w, ypos, 1.0, 1.0},

        {xpos, ypos + h, 0.0, 0.0},    {xpos + w, ypos, 1.0, 1.0},
        {xpos + w, ypos + h, 1.0, 0.0}};
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64
    // pixels)
    x += (ch.advance >> 6) *
         scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
