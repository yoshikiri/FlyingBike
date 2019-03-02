#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "DrawFigure.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"

int main() {
  //--------------------------------------------------------------------------//
  // init settings
  if (glfwInit() == GL_FALSE) {
    std::cerr << "Cannot init GLFW" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Window window;
  const float WIDTH = 8.0f;
  const float HEIGHT = 6.0f;

  atexit(glfwTerminate);

  // 背景色設定
  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

  // 背面カリングを有効にする
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  // textureを上下逆に読み込む
  stbi_set_flip_vertically_on_load(true);

  // デプスバッファを有効にする
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // shaderを作成する
  Shader shader("../vertexshader.vert", "../fragmentshader.frag");

  // modelの読み込み
  char nanosuit_path[] = "resource/nanosuit/nanosuit.obj";
  Model myModel(nanosuit_path);

  //--------------------------------------------------------------------------//
  // メインループ
  while (window.shouldClose() == GL_FALSE) {
    // processInput(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    // 直交投影
    const glm::mat4 projection(
        glm::ortho(-WIDTH, WIDTH, -HEIGHT, HEIGHT, 0.0f, 10.0f));

    // 透視投影変換
    // const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

    glm::mat4 model(1.0);
    const float sc = 0.7f;
    glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(sc, sc, sc));
    glm::mat4 trans =
        glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -8.0f, 0.0f));
    model = scale * trans;
    glm::mat4 view(glm::lookAt(glm::vec3(1.0f, 0.5f, 2.0f),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f)));

    // uniform変数に値をセットする
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // 描画処理
    // double x, y;
    // glfwGetCursorPos(window.getWindow(), &x, &y);
    // x = (x / (WIDTH * 50) - 1) * WIDTH;
    // y = (y / (HEIGHT * 50) - 1) * -HEIGHT;

    myModel.draw(shader);

    // ダブルバッファリング
    window.swapBuffers();
  }

  return 0;
}
