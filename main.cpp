#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Camera.h"
#include "DrawFigure.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"
#include "Constants.h"

unsigned int loadTextureM(std::string filename, bool rgbaFlag = false);

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
  const float WIDTH = 800.0f;
  const float HEIGHT = 600.0f;

  atexit(glfwTerminate);

  // 背景色設定
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

  // 背面カリングを有効にする
  // glFrontFace(GL_CCW);
  // glCullFace(GL_BACK);
  // glEnable(GL_CULL_FACE);

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

  // glm::vec3 center(0, 0, 0);
  // glm::vec3 length(2, 2, 2);
  // const float cubeVertex[] = {
  //     center.x - length.x / 2, center.y - length.y / 2, center.z - length.z / 2,
  //     // 0
  //     center.x + length.x / 2, center.y - length.y / 2, center.z - length.z / 2,
  //     // 1
  //     center.x + length.x / 2, center.y + length.y / 2, center.z - length.z / 2,
  //     // 2
  //     center.x - length.x / 2, center.y + length.y / 2, center.z - length.z / 2,
  //     // 3
  //     center.x - length.x / 2, center.y - length.y / 2, center.z + length.z / 2,
  //     // 4
  //     center.x + length.x / 2, center.y - length.y / 2, center.z + length.z / 2,
  //     // 5
  //     center.x + length.x / 2, center.y + length.y / 2, center.z + length.z / 2,
  //     // 6
  //     center.x - length.x / 2, center.y + length.y / 2, center.z + length.z / 2,
  //     // 7
  // };
  // const std::vector<Vertex> cubeVertexV = {
  //     {glm::vec3(center.x - length.x / 2, center.y - length.y / 2, center.z - length.z / 2), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 0.0f,  0.0f)},
  //     // 0
  //     {glm::vec3(center.x + length.x / 2, center.y - length.y / 2, center.z - length.z / 2), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  0.0f)},
  //     // 1
  //     {glm::vec3(center.x + length.x / 2, center.y + length.y / 2, center.z - length.z / 2), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  1.0f)},
  //     // 2
  //     {glm::vec3(center.x - length.x / 2, center.y + length.y / 2, center.z - length.z / 2), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 0.0f,  1.0f)},
  //     // 3
  //     {glm::vec3(center.x - length.x / 2, center.y - length.y / 2, center.z + length.z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
  //     // 4
  //     {glm::vec3(center.x + length.x / 2, center.y - length.y / 2, center.z + length.z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
  //     // 5
  //     {glm::vec3(center.x + length.x / 2, center.y + length.y / 2, center.z + length.z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
  //     // 6
  //     {glm::vec3(center.x - length.x / 2, center.y + length.y / 2, center.z + length.z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)}
  //     // 7
  // };

  // float vertices[] = {
  //        // positions          // normals           // texture coords
  //        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
  //         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
  //         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
  //         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
  //        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
  //        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
  //
  //        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
  //         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
  //         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
  //         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
  //        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
  //        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
  //
  //        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  //        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
  //        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  //        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  //        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  //        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  //
  //         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  //         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
  //         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  //         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  //         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  //         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  //
  //        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
  //         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
  //         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  //         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  //        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
  //        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
  //
  //        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
  //         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
  //         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  //         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  //        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
  //        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
  //    };

  // float *v = Constants::VERTICES_CUBE_FLOAT;
  std::vector<Vertex> verticesVV;

  for(int i=0;i<8*36;i+=8){
    glm::vec3 pos(Constants::VERTICES_CUBE_FLOAT[i], Constants::VERTICES_CUBE_FLOAT[i+1], Constants::VERTICES_CUBE_FLOAT[i+2]);
    glm::vec3 nor(Constants::VERTICES_CUBE_FLOAT[i+3], Constants::VERTICES_CUBE_FLOAT[i+4], Constants::VERTICES_CUBE_FLOAT[i+5]);
    glm::vec2 tex(Constants::VERTICES_CUBE_FLOAT[i+6], Constants::VERTICES_CUBE_FLOAT[i+7]);
    Vertex v = {pos, nor, tex};
    verticesVV.push_back(v);
  }

  Figure fig(GL_TRIANGLES, true, true, verticesVV);
  Figure light(GL_TRIANGLES, false, false, verticesVV);


  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  // unsigned int VBO, cubeVAO;
  // glGenVertexArrays(1, &cubeVAO);
  // glGenBuffers(1, &VBO);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //
  // glBindVertexArray(cubeVAO);
  //
  // // position attribute
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  // glEnableVertexAttribArray(0);
  // // normal attribute
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);
  //
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
  // glEnableVertexAttribArray(2);
  // // second, configure the light's VAO (VBO stays the same; the vertices are the
  // // same for the light object which is also a 3D cube)
  // unsigned int lightVAO;
  // glGenVertexArrays(1, &lightVAO);
  // glBindVertexArray(lightVAO);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // // note that we update the lamp's position attribute's stride to reflect the
  // // updated buffer data
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  // glEnableVertexAttribArray(0);

  unsigned int diffuseMap = loadTextureM("resource/container2.png", true);
  unsigned int specularMap = loadTextureM("resource/container2_specular.png", true);

  glm::vec3 lampPos(1.2f, 1.0f, 1.0f);

  Shader lightingShader("../lightingVertexShader.vert", "../lightingFragmentShader.frag");
  lightingShader.use();

  Shader lampShader("../lampVertexShader.vert", "../lampFragmentShader.frag");
  glm::mat4 lampModel(1.0f);
  lampModel = glm::translate(glm::mat4(1.0), lampPos);
  lampModel = glm::scale(lampModel, glm::vec3(0.2f));


  Camera camera(glm::vec3(5.5f, -5.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f), WIDTH, HEIGHT);

  //--------------------------------------------------------------------------//
  // メインループ
  double t = 0;
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 透視投影変換
    glm::mat4 model(1.0);
    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getProjection();


    double radius = 2;
    glm::vec3 pos(radius * cos(glfwGetTime()), radius * sin(glfwGetTime()), 1.0);
    // glm::vec3 pos(3.0);

    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);

    lightingShader.setVec3("viewPos", camera.getPosition());

    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    lightingShader.setVec3("light.position", pos);
    lightingShader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("light.constant", 1.0f);
    lightingShader.setFloat("light.linear", 0.09f);
    lightingShader.setFloat("light.quadratic", 0.032f);
    // lightingShader.setVec3("light.diffuse", lightColor * glm::vec3(0.2));

    // 描画処理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    fig.draw();
    // cube.draw();
    for(int i=0;i<10;i++){
      model = glm::mat4(1.0);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, glm::radians((float)i), glm::vec3(1.0, 0.3, 0.5));
      lightingShader.setMat4("model", model);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffuseMap);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specularMap);
      fig.draw();
      // glBindVertexArray(cubeVAO);
      // glDrawArrays(GL_TRIANGLES, 0, 36);
      // glBindVertexArray(0);
    }

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBindVertexArray(cubeVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);
    // glBindVertexArray(0);
    // lampModel = glm::rotate(lampModel, (float)M_PI/4, glm::vec3(0, 0, 1));

    lampModel = glm::translate(glm::mat4(1.0), pos);
    lampModel = glm::scale(lampModel, glm::vec3(0.2));
    // lampModel = glm::mat4(1.0f);
    lampShader.use();
    lampShader.setMat4("model", lampModel);
    lampShader.setMat4("view", view);
    lampShader.setMat4("projection", projection);
    t += 0.03;

    light.draw();

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBindVertexArray(lightVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);

    // double x, y;
    // glfwGetCursorPos(window.getWindow(), &x, &y);
    // x = (x / (WIDTH * 50) - 1) * WIDTH;
    // y = (y / (HEIGHT * 50) - 1) * -HEIGHT;

    // DrawFigure::drawCube(glm::vec3(0, 0, 0), glm::vec3(2, 2, 2), 0.5, true);
    // myModel.draw(shader);

    // ダブルバッファリング
    window.swapBuffers();
  }

  return 0;
}


unsigned int loadTextureM(std::string filename, bool rgbaFlag) {
  int WIDTH, HEIGHT, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &WIDTH, &HEIGHT, &nrChannels, STBI_rgb_alpha);
  if (data == 0) {
    std::cout << "cannot load " << filename << '\n';
  }

  unsigned int texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (rgbaFlag == false)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  if (rgbaFlag == true)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}
