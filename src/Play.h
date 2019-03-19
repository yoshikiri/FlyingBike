#ifndef PLAY_H
#define PLAY_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

#include "State.h"

class Player;
class Container;
class Goal;
class Shader;
class Camera;

namespace std {
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T> class allocator;
template <class T, class Allocator> class vector;
} // namespace std


class Play : public State {
public:
  Play(GLFWwindow *window, unsigned int stage);

  ~Play();

  State *update() override;

private:
  std::unique_ptr<Player> player;
  std::unique_ptr<std::vector<Container, std::allocator<Container>>> containers;
  std::unique_ptr<std::vector<Goal, std::allocator<Goal>>> goals;
  std::unique_ptr<Shader> objectShader;
  std::unique_ptr<Shader> skyboxShader;
  std::unique_ptr<Shader> textShader;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<unsigned int[]> objectTextures;
  std::unique_ptr<unsigned int[]> numberTextures;
  const unsigned int stage;
  bool isSettingTarget;
  float startTime;

  void draw();
  void updatePlayer();
  State *checkGameEnd();
  void initShaders();
  void updateShaders();
};

#endif