#ifndef TITLE_H
#define TITLE_H

#include <GL/glew.h>

#include "State.h"

class Shader;
class Camera;

class Title : public State {
public:
  Title(GLFWwindow *window);

  State *update() override;

private:
  std::unique_ptr<Shader> lightShader;
  std::unique_ptr<Shader> objectShader;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<unsigned int[]> numberTextures;
  std::unique_ptr<unsigned int[]> titleTextures;
  int stage;

  void draw();
  void initShaders();
  void updateShaders();
};

#endif