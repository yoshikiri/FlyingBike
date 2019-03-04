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
unsigned int loadCubeMap(std::vector<std::string> faces);

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
  // stbi_set_flip_vertically_on_load(true);

  // デプスバッファを有効にする
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // shaderを作成する
  Shader shader("../vertexshader.vert", "../fragmentshader.frag");

  // modelの読み込み
  char nanosuit_path[] = "resource/nanosuit/nanosuit.obj";
  Model myModel(nanosuit_path);

  std::vector<Vertex> verticesV;

  for(int i=0;i<8*36;i+=8){
    glm::vec3 pos(Constants::VERTICES_CUBE_FLOAT[i], Constants::VERTICES_CUBE_FLOAT[i+1], Constants::VERTICES_CUBE_FLOAT[i+2]);
    glm::vec3 nor(Constants::VERTICES_CUBE_FLOAT[i+3], Constants::VERTICES_CUBE_FLOAT[i+4], Constants::VERTICES_CUBE_FLOAT[i+5]);
    glm::vec2 tex(Constants::VERTICES_CUBE_FLOAT[i+6], Constants::VERTICES_CUBE_FLOAT[i+7]);
    Vertex v = {pos, nor, tex};
    verticesV.push_back(v);
  }

  std::vector<Vertex> skyboxVertices;
  for(int i=0;i<3*36;i+=3){
    glm::vec3 pos(Constants::VERTICES_SKYBOX_FLOAT[i], Constants::VERTICES_SKYBOX_FLOAT[i+1], Constants::VERTICES_SKYBOX_FLOAT[i+2]);
    glm::vec3 nor(0);
    glm::vec2 tex(0);
    Vertex v = {pos, nor, tex};
    skyboxVertices.push_back(v);
  }

  Figure fig(GL_TRIANGLES, true, true, verticesV);
  Figure light(GL_TRIANGLES, false, false, verticesV);
  Figure skybox(GL_TRIANGLES, false, false, skyboxVertices);

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

  unsigned int diffuseMap = loadTextureM("resource/container2.png", true);
  unsigned int specularMap = loadTextureM("resource/container2_specular.png", true);

  std::vector<std::string> faces = {"resource/skybox/right.jpg", "resource/skybox/left.jpg", "resource/skybox/top.jpg", "resource/skybox/bottom.jpg", "resource/skybox/front.jpg", "resource/skybox/back.jpg"};
  unsigned int cubeMap = loadCubeMap(faces);

  glm::vec3 lampPos(1.2f, 1.0f, 1.0f);

  Shader lightingShader("../lightingVertexShader.vert", "../lightingFragmentShader.frag");
  lightingShader.use();

  Shader lampShader("../lampVertexShader.vert", "../lampFragmentShader.frag");
  glm::mat4 lampModel(1.0f);
  lampModel = glm::translate(glm::mat4(1.0), lampPos);
  lampModel = glm::scale(lampModel, glm::vec3(0.2f));

  Shader skyboxShader("../skyboxVertexShader.vert", "../skyboxFragmentShader.frag");


  Camera camera(glm::vec3(20.0f, 10.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f), WIDTH, HEIGHT);

  //--------------------------------------------------------------------------//
  // メインループ
  double t = 0;
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 透視投影変換
    glm::mat4 model(1.0);
    // model = glm::scale(model, glm::vec3(0.2f));
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
    myModel.draw(lightingShader);
    //
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
    }
    //
    lampModel = glm::translate(glm::mat4(1.0), pos);
    lampModel = glm::scale(lampModel, glm::vec3(0.2));
    // lampModel = glm::mat4(1.0f);
    lampShader.use();
    lampShader.setMat4("model", lampModel);
    lampShader.setMat4("view", view);
    lampShader.setMat4("projection", projection);
    t += 0.03;
    //
    light.draw();

    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    // skyboxShader.setMat4("model", glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1,0,0)));
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    skyboxShader.setInt("skybox", 0);

    skybox.draw();
    glDepthFunc(GL_LESS);

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

  unsigned int textureID = 0;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

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

  return textureID;
}

unsigned int loadCubeMap(std::vector<std::string> faces){
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int WIDTH, HEIGHT, nrChannels;
  for(unsigned int i=0;i<faces.size();i++){
    unsigned char *data =
    stbi_load(faces[i].c_str(), &WIDTH, &HEIGHT, &nrChannels, 0);
    if (data == 0) {
      std::cout << "cannot load " << faces[i] << '\n';
    }
    else{
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return textureID;
}
