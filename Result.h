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
  std::unique_ptr<Shader> lightShader;
  std::unique_ptr<Shader> objectShader;
  std::unique_ptr<Shader> textShader;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<unsigned int[]> numberTextures;
  std::unique_ptr<unsigned int[]> resultTextures;
  // const std::unique_ptr<Figure> number;
  const float score;
  std::unique_ptr<float[]> highScores;
  const bool isClear;
  const unsigned int stage;
  bool updateHighScore;

  void draw();
  void initShaders();
  void updateShaders();
  void drawScore(glm::vec2 pos, float score);
  void loadHighScore(std::string filepath);
  void writeHighScore(std::string filepath);
  void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y,
                  GLfloat scale, glm::vec3 color);
};

#endif