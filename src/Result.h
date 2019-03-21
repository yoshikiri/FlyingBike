#ifndef RESULT_H
#define RESULT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

#include "State.h"

class Shader;
class Camera;
class Figure;

class Result : public State {
public:
  Result(GLFWwindow *window, float score, bool isClear, unsigned int stage);

  State *update() override;

private:
  std::unique_ptr<Shader> objectShader;
  std::unique_ptr<Camera> camera;

  std::unique_ptr<unsigned int[]> resultTextures;
  std::unique_ptr<unsigned int[]> numberTextures;
  std::unique_ptr<float[]> highScores;
  const float score;
  const bool isClear;
  const unsigned int stage;
  bool updateHighScore;

  void draw() const;
  void drawScore(const glm::vec2 pos, const float score) const;
  void loadHighScore(const std::string filepath);
  void writeHighScore(const std::string filepath) const;
  void initShaders();
  void updateShaders();
};

#endif