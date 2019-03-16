#include "Play.h"

// #include <glm/glm.hpp>
#include <cstdio>
#include <stb_image.h>
#include <unistd.h>
#include <map>
#include <vector>

#include "Camera.h"
#include "Container.h"
#include "DrawFigure.h"
#include "Goal.h"
#include "Player.h"
#include "Result.h"
#include "Shader.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

struct Character {
  GLuint textureID;
  glm::ivec2 size;
  glm::ivec2 bearing;
  GLuint advance;
};


namespace {
std::map<GLchar, Character> characters;


int target = 0;
std::vector<Goal> targets;

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
const int MAX_TARGETS = 10;

const std::string fileObjectTextures[][6] = {
    {"resource/poly.png", "resource/container2_specular.png",
     "resource/container2.png", "resource/container2_specular.png",
     "resource/wood-texture.png", "resource/one.png"},

    {"resource/poly.png", "resource/container2_specular.png",
     "resource/container2.png", "resource/container2_specular.png",
     "resource/wood-texture.png", "resource/one.png"},
};

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (targets.size() >= MAX_TARGETS)
      return;
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    float tx = -(x - 400) / 50.0f;
    float ty = (y - 300) / 50.0f;
    targets.emplace_back(glm::vec3(tx, ty, 0.5f), 0.1);
  }
}

unsigned int loadTexture(std::string filename, bool useAlpha = true) {
  stbi_set_flip_vertically_on_load(true);

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

Play::Play(GLFWwindow *window, glm::vec3 center, unsigned int stage)
    : State(window),

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
      textShader(std::make_unique<Shader>("resource/shader/text.vert",
                                          "resource/shader/text.frag")),
      camera(std::make_unique<Camera>(cameraPositionLookDown, cameraEye,
                                      cameraUp, WIDTH, HEIGHT, true)),
      objectTextures(std::make_unique<unsigned int[]>(6)),
      numberTextures(std::make_unique<unsigned int[]>(10)), stage(stage),
      isSettingTarget(true) {
  //--------------------------------------------------------------------------//

  for (int i = 0; i < 6; i++) {
    objectTextures[i] = loadTexture(fileObjectTextures[stage][i]);
  }

  for (int i = 0; i < 10; i++) {
    char buff[5];
    snprintf(buff, sizeof(buff), "%d", i);
    numberTextures[i] = loadTexture(
        std::string("resource/number/") + std::string(buff) + ".png", true);
  }

  player =
      std::make_unique<Player>(center, objectTextures[0], objectTextures[1]);

  for (glm::vec3 p : containerPositions)
    containers->emplace_back(p, glm::vec3(1.0f), objectTextures[2],
                             objectTextures[3]);
  for (glm::vec3 p : goalPositions)
    goals->emplace_back(p, 0.4f);

  target = 0;
  targets.clear();

  initShaders();

  glfwSetMouseButtonCallback(window, mouse_button_callback);

  // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  //   player->velocity.x += 0.001;
  // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  //   player->velocity.x -= 0.001;
  // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  //   player->velocity.y += 0.001;
  // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  //   player->velocity.y -= 0.001;

  // textShader->setMat4("projection", glm::ortho(-40.0f, 40.0f, -30.0f, 30.0f));
  textShader->setMat4("projection", camera->getProjection());
}

State *Play::update() {
  State *next = this;

  if (isSettingTarget) {
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      isSettingTarget = false;
      glfwSetMouseButtonCallback(window, 0);
    }
  } else {
    updatePlayer();
    next = checkGameEnd();
  }

  draw();
  updateShaders();
  // textShader->setMat4("projection", camera->getProjection());
  return next;
}

void Play::draw() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // draw containers
  for (Container c : *containers)
    c.draw();

  // draw planes
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, objectTextures[4]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      DrawFigure::drawPlane(glm::vec3(-10 + x, -10 + y, 0), glm::vec2(1, 1));
    }
  }

  // draw targets
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0);

  for (int i = 0; i < targets.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, numberTextures[i]);
    DrawFigure::drawCircle(targets[i].center, 0.6, 20);
  }

  // draw player
  player->draw();

  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // RenderText(*textShader, "This is sample text", 0.0f, 0.0f, 1.0f,
  //            glm::vec3(0.5f, 0.8f, 0.2f));
  // RenderText(*textShader, "(C) LearnOpenGL.com", 2.0f, 2.0f, 0.5f,
  //            glm::vec3(0.3, 0.7f, 0.9f));
}

void Play::updatePlayer() {
  if (targets.size() != 0)
    player->seek(targets[target].center);

  // check if player's collision
  for (Container c : *containers)
    player->checkCollision(c);

  for (Goal &g : *goals)
    player->checkGoal(g);

  // change target
  player->checkGoal(targets[target]);
  if (targets.size() != 0 && targets[target].isClear == true) {
    target++;
    target = (target >= targets.size()) ? (targets.size() - 1) : target;
  }

  player->update();
}

State *Play::checkGameEnd() {
  State *next = this;
  bool isEnd = true;

  for (Goal t : targets) {
    if (!t.isClear)
      isEnd = false;
  }

  if (isEnd) {
    sleep(2);
    bool gameClear = true;

    for (Goal g : *goals) {
      if (!g.isClear)
        gameClear = false;
    }
    next = new Result(window, glfwGetTime(), gameClear, stage);
  }

  return next;
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

void Play::RenderText(Shader &s, std::string text, GLfloat x, GLfloat y,
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
  textShader->setMat4("projection", camera->getProjection());

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

    // // position
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
    //                       (void *)(0));
    // glEnableVertexAttribArray(0);
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
