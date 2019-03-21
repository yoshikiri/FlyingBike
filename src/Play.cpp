#include "Play.h"

#include <cstdio>
#include <irrKlang/irrKlang.h>
#include <map>
#include <stb_image.h>
#include <unistd.h>
#include <vector>

#include "Camera.h"
#include "Container.h"
#include "DrawFigure.h"
#include "Goal.h"
#include "Player.h"
#include "Result.h"
#include "Shader.h"

namespace {
const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

const glm::vec3 cameraPositionLookDown(0.0f, 0.0f, 10.0f);
const glm::vec3 cameraEye(0.0f, 0.0f, 0.0f);
const glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

auto soundEngine = std::make_unique<irrklang::ISoundEngine *>(
    irrklang::createIrrKlangDevice());

const glm::vec3 playerStartPosition[] = {
    glm::vec3(4.0f, 2.5f, 0.5f),
    glm::vec3(-5.0f, 3.0f, 0.5f),
    glm::vec3(-3.0f, -3.5f, 0.5f),
    glm::vec3(0.0f, 0.0f, 0.5f),
};

const glm::vec3 containerPositions[][4] = {

    {glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(2.5f, 1.5f, 0.5f),
     glm::vec3(-1.0f, 1.0f, 0.5f), glm::vec3(-2.0f, -2.0f, 0.5f)},

    {glm::vec3(1.0f, -1.0f, 0.5f), glm::vec3(1.0f, 0.5f, 0.5f),
     glm::vec3(1.0f, 2.0f, 0.5f), glm::vec3(1.0f, 3.5f, 0.5f)},

    {glm::vec3(1.0f, 0.0f, 0.5f), glm::vec3(2.0f, 1.0f, 0.5f),
     glm::vec3(-3.0f, 2.0f, 0.5f), glm::vec3(4.0f, 3.0f, 0.5f)},

    {glm::vec3(-3.0f, -3.0f, 0.5f), glm::vec3(-3.0f, 3.0f, 0.5f),
     glm::vec3(3.0f, -3.0f, 0.5f), glm::vec3(3.0f, 3.0f, 0.5f)},
};

const glm::vec3 goalPositions[][4] = {
    {glm::vec3(-4.5f, -2.0f, 0.5f), glm::vec3(3.5f, -2.5f, 0.5f),
     glm::vec3(-3.5f, 0.5f, 0.5f), glm::vec3(-2.0f, 3.0f, 0.5f)},

    {glm::vec3(3.0f, 0.0f, 0.5f), glm::vec3(3.0f, 1.0f, 0.5f),
     glm::vec3(3.0f, 2.0f, 0.5f), glm::vec3(3.0f, 3.0f, 0.5f)},

    {glm::vec3(1.0f, 1.0f, 0.5f), glm::vec3(2.0f, 3.0f, 0.5f),
     glm::vec3(2.5f, -2.0f, 0.5f), glm::vec3(5.0f, -5.0f, 0.5f)},

    {glm::vec3(-3.0f, 0.0f, 0.5f), glm::vec3(0.0f, 3.0f, 0.5f),
     glm::vec3(3.0f, 0.0f, 0.5f), glm::vec3(0.0f, -3.0f, 0.5f)},
};

const unsigned int SCENE_CHANGE_DELAY = 2;

const int NUM_SPOT_LIGHTS = 4;
const int MAX_TARGETS = 10;
const float TARGET_DRAW_RADIUS = 0.5f;
const float TARGET_RADIUS = 0.01f;
const int TARGET_DRAW_VERTICES_NUM = 20;
const float CONTAINER_LENGTH = 1.0f;
const float GOAL_RADIUS = 0.6f;
const int COL = 20;
const int ROW = 20;
const float PLANE_LENGTH = 1.0f;

std::map<std::string, int> TEXTURE_INDEX = {{"PLAYER_AMBIENT", 0},
                                            {"PLAYER_SPECECLAR", 1},
                                            {"CONTAINER_AMBIENT", 2},
                                            {"CONTA_SPECECLAR", 3},
                                            {"PLANE_AMBIENT", 4}};

const std::string fileObjectTextures[][5] = {
    {"resource/image/star_sky.png", "resource/image/star_sky.png",
     "resource/image/danger_box.png", "resource/image/danger_box.png",
     "resource/image/brick_four_tile_shadow.png"},

    {"resource/image/star_sky.png", "resource/image/star_sky.png",
     "resource/image/danger_box.png", "resource/image/danger_box.png",
     "resource/image/brick_four_tile_shadow.png"},

    {"resource/image/star_sky.png", "resource/image/star_sky.png",
     "resource/image/danger_box.png", "resource/image/danger_box.png",
     "resource/image/brick_four_tile_shadow.png"},

    {"resource/image/star_sky.png", "resource/image/star_sky.png",
     "resource/image/danger_box.png", "resource/image/danger_box.png",
     "resource/image/brick_four_tile_shadow.png"},
};

int target = 0;
std::vector<Goal> targets;
float startTime = 0;

// add or delete target
void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  // add target at the mouse cursor position
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (targets.size() >= MAX_TARGETS)
      return;
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    float tx = -(x - WIDTH / 2.0f) / 50.0f;
    float ty = (y - HEIGHT / 2.0f) / 50.0f;
    tx /= double(1.0 + abs(tx) / (WIDTH / 10.0f));
    ty /= double(1.0 + abs(ty) / (HEIGHT / 10.0f));

    targets.emplace_back(glm::vec3(tx, ty, TARGET_DRAW_RADIUS), TARGET_RADIUS);
    (*soundEngine)->play2D("resource/sound/se_add_target.ogg", false);
  }

  // delete last added target
  else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    if (targets.size() > 0) {
      targets.erase(targets.end() - 1);
    }
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

Play::Play(GLFWwindow *window, unsigned int stage)
    : State(window),

      objectShader(std::make_unique<Shader>(
          "resource/shader/directionalAndSpotLightObject.vert",
          "resource/shader/directionalAndSpotLightObject.frag")),
      camera(std::make_unique<Camera>(cameraPositionLookDown, cameraEye,
                                      cameraUp, WIDTH, HEIGHT, true)),
      containers(std::make_unique<
                 std::vector<Container, std::allocator<Container>>>()),
      goals(std::make_unique<std::vector<Goal, std::allocator<Goal>>>()),
      objectTextures(std::make_unique<unsigned int[]>(TEXTURE_INDEX.size())),
      numberTextures(std::make_unique<unsigned int[]>(10)), stage(stage),
      isSettingTarget(true) {
  //--------------------------------------------------------------------------//

  // TEXTURE_INDEX.size();
  for (int i = 0; i < TEXTURE_INDEX.size(); i++) {
    objectTextures[i] = loadTexture(fileObjectTextures[stage][i]);
  }

  for (int i = 0; i < 10; i++) {
    char buff[5];
    snprintf(buff, sizeof(buff), "%d", i);
    numberTextures[i] = loadTexture(std::string("resource/image/target/") +
                                        std::string(buff) + ".png",
                                    true);
  }

  // create player, containers, goals
  player = std::make_unique<Player>(
      playerStartPosition[stage],
      objectTextures[TEXTURE_INDEX["PLAYER_AMBIENT"]],
      objectTextures[TEXTURE_INDEX["PLAYER_SPECECLAR"]]);

  for (glm::vec3 p : containerPositions[stage])
    containers->emplace_back(
        p, glm::vec3(CONTAINER_LENGTH),
        objectTextures[TEXTURE_INDEX["CONTAINER_AMBIENT"]],
        objectTextures[TEXTURE_INDEX["CONTAINER_AMBIENT"]]);

  for (glm::vec3 p : goalPositions[stage])
    goals->emplace_back(p, GOAL_RADIUS);

  // init target
  target = 0;
  targets.clear();

  startTime = glfwGetTime();

  initShaders();

  glfwSetMouseButtonCallback(window, mouse_button_callback);
}

Play::~Play() {
  (*soundEngine)->stopAllSounds();
  (*soundEngine)->drop();
}

State *Play::update() {
  State *next = this;

  // cancel the game
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    next = new Result(window, (glfwGetTime() - startTime), false, stage);

  if (isSettingTarget) { // taret settig phase
    if (targets.size() > 0 &&
        glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
      isSettingTarget = false;
      glfwSetMouseButtonCallback(window, 0);
      (*soundEngine)->play2D("resource/sound/se_start_moving.ogg", false);
      startTime = glfwGetTime();
    }
  } else { // moving phase
    updatePlayer();
    next = checkGameEnd();
  }

  updateShaders();
  draw();
  return next;
}

void Play::draw() const {
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // draw planes
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, objectTextures[TEXTURE_INDEX["PLANE_AMBIENT"]]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  for (int y = 0; y < COL; y++) {
    for (int x = 0; x < ROW; x++) {
      DrawFigure::drawPlane(glm::vec3(-ROW / 2 + x, -COL / 2 + y, 0),
                            glm::vec2(PLANE_LENGTH, PLANE_LENGTH));
    }
  }

  // draw containers
  for (Container c : *containers)
    c.draw();

  // draw targets
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0);

  for (int i = 0; i < targets.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, numberTextures[i]);
    DrawFigure::drawCircle(targets[i].center, TARGET_DRAW_RADIUS,
                           TARGET_DRAW_VERTICES_NUM);
  }

  // draw player
  player->draw();
}

// process player's move and collision (containers and targets)
void Play::updatePlayer() {
  if (targets.size() != 0)
    player->seek(targets[target].center);

  // check  player's collision
  for (Container c : *containers)
    player->checkCollision(c);

  for (Goal &g : *goals) {
    bool prevIsClear = g.isClear;

    if (player->checkGoal(g)) {
      // play sound once when reach goal
      if (g.isClear == prevIsClear)
        continue;

      (*soundEngine)->play2D("resource/sound/se_check_goal.ogg", false);
    }
  }

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

  // game is end if all targets are checked
  for (Goal t : targets) {
    if (!t.isClear)
      isEnd = false;
  }

  // game is clear if all goals are checked
  if (isEnd) {
    bool gameClear = true;
    for (Goal g : *goals) {
      if (!g.isClear)
        gameClear = false;
    }
    next = new Result(window, (glfwGetTime() - startTime), gameClear, stage);
    sleep(SCENE_CHANGE_DELAY);
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
    objectShader->setVec3(std::string(buff), goalPositions[stage][i] +
                                                 glm::vec3(0.0f, 0.0f, 1.5f));
    snprintf(buff, sizeof(buff), "spotLights[%d].direction", i);
    objectShader->setVec3(std::string(buff), glm::vec3(0.0f, 0.0f, -1.0f));
    snprintf(buff, sizeof(buff), "spotLights[%d].ambient", i);
    objectShader->setVec3(std::string(buff), (*goals)[i].color);
    snprintf(buff, sizeof(buff), "spotLights[%d].diffuse", i);
    objectShader->setVec3(std::string(buff), (*goals)[i].color);
    snprintf(buff, sizeof(buff), "spotLights[%d].specular", i);
    objectShader->setVec3(std::string(buff), (*goals)[i].color);
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
    // if a goal is cleared, delete light by changing direction
    snprintf(buff, sizeof(buff), "spotLights[%d].direction", i);
    objectShader->setVec3(std::string(buff),
                          (*goals)[i].isClear ? glm::vec3(0.0f, 0.0f, 1.0f)
                                              : glm::vec3(0.0f, 0.0f, -1.0f));
  }
}
