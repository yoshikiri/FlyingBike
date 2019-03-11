#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Camera.h"
#include "Constants.h"
#include "DrawFigure.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"
// #include "PerlinNoise.h"
// #include "ppm.h"

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

  const float WIDTH = 800.0f;
  const float HEIGHT = 600.0f;
  Window window(WIDTH, HEIGHT, "Flying Bike");

  atexit(glfwTerminate);

  // set background color
  // glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
  glClearColor(0.9f, 0.9f, 0.9f, 0.0f);

  // enable culling
  // glFrontFace(GL_CCW);
  // glCullFace(GL_BACK);
  // glEnable(GL_CULL_FACE);

  // load texture flip flop vertically
  stbi_set_flip_vertically_on_load(true);

  // enable depth buffer
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  //--------------------------------------------------------------------------//
  // load model
  char nanosuit_path[] = "resource/nanosuit/nanosuit.obj";
  Model myModel(nanosuit_path);

  // std::vector<Vertex> skyboxVertices;
  // for (int i = 0; i < 3 * 36; i += 3) {
  //   glm::vec3 pos(Constants::VERTICES_SKYBOX_FLOAT[i],
  //                 Constants::VERTICES_SKYBOX_FLOAT[i + 1],
  //                 Constants::VERTICES_SKYBOX_FLOAT[i + 2]);
  //   glm::vec3 nor(0);
  //   glm::vec2 tex(0);
  //   Vertex v = {pos, nor, tex};
  //   skyboxVertices.push_back(v);
  // }

  //--------------------------------------------------------------------------//
  // load textures

  unsigned int diffuseMap = loadTextureM("resource/container2.png", true);
  // unsigned int specularMap = loadTextureM("resource/wall.jpg", false);
  unsigned int specularMap =
      loadTextureM("resource/container2_specular.png", true);

  // unsigned int triMap = loadTextureM("resource/wall.jpg", false);
  //
  // std::vector<std::string> faces = {
  //     "resource/skybox/right.jpg", "resource/skybox/left.jpg",
  //     "resource/skybox/top.jpg",   "resource/skybox/bottom.jpg",
  //     "resource/skybox/front.jpg", "resource/skybox/back.jpg"};
  // unsigned int cubeMap = loadCubeMap(faces);

  //--------------------------------------------------------------------------//
  // create shaders
  Shader lampShader("../lampVertexShader.vert", "../lampFragmentShader.frag");

  // object shader
  Shader lightingShader("../directionalLight.vert", "../directionalLight.frag");
  // Shader lightingShader("../pointLight.vert", "../pointLight.frag");
  // Shader lightingShader("../spotLight.vert", "../spotLight.frag");

  Shader skyboxShader("../skyboxVertexShader.vert",
                      "../skyboxFragmentShader.frag");

  //--------------------------------------------------------------------------//
  // create camera (perspective)
  Camera camera(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT, true);

  //--------------------------------------------------------------------------//
  // main loop
  double t = 0;
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //------------------------------------------------------------------------//
    // draw light
    // double radius = 1;
    // glm::vec3 pos(radius * cos(glfwGetTime()), radius * sin(glfwGetTime()),
    //               -3.0);
    // glm::vec3 pos(1.0, 1.0, -3.0);
    glm::vec3 lampPos(1.0f, 1.0f, 3.0f);
    glm::mat4 lampModel(glm::translate(glm::mat4(1.0f), lampPos));
    glm::mat4 lampView = camera.getView();
    glm::mat4 lampProjection = camera.getProjection();
    lampShader.use();
    lampShader.setMat4("model", lampModel);
    lampShader.setMat4("view", lampView);
    lampShader.setMat4("projection", lampProjection);
    t += 0.03;

    DrawFigure::drawCube(glm::vec3(1), glm::vec3(0.5), false, false);

    //------------------------------------------------------------------------//
    // set attribute for object shader
    glm::mat4 model(1.0);
    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getProjection();

    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);

    lightingShader.setVec3("viewPos", camera.getPosition());

    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    // lightingShader.setVec3("light.position", lampPos);
    lightingShader.setVec3("light.direction", glm::vec3(0.0f, 0.0f, 0.3f));
    // lightingShader.setVec3("light.direction", glm::vec3(0.2f, 1.0f, 0.3f));
    // lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    // lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

    lightingShader.setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    lightingShader.setVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // set attribute when the object shader uses point light
    // lightingShader.setFloat("light.constant", 1.0f);
    // lightingShader.setFloat("light.linear", 0.09f);
    // lightingShader.setFloat("light.quadratic", 0.032f);

    //------------------------------------------------------------------------//
    // draw objects

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, triMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // fig.draw();
    DrawFigure::drawCube(glm::vec3(0, 0, 0.5), glm::vec3(1));
    DrawFigure::drawCube(glm::vec3(0, 2, 0.5), glm::vec3(1));
    DrawFigure::drawCube(glm::vec3(-1, 0, 0.5), glm::vec3(1));
    // plane.draw();

    for (int y = 0; y < 10; y++) {
      for (int x = 0; x < 10; x++) {
        DrawFigure::drawPlane(glm::vec3(-4 + x, -4 + y, 0), glm::vec2(1, 1));
      }
    }

    // model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0));
    // lightingShader.setMat4("model", model);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // plane.draw();

    // myModel.draw(lightingShader);

    //------------------------------------------------------------------------//
    // draw skybox

    // glDepthFunc(GL_LEQUAL);
    // skyboxShader.use();
    // skyboxShader.setMat4("model", glm::rotate(glm::mat4(1.0), -90.0f,
    // glm::vec3(1,0,0)));
    // skyboxShader.setMat4("projection", projection);
    // skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    // skyboxShader.setInt("skybox", cubeMap);
    //
    // // skybox.draw();
    // glDepthFunc(GL_LESS);

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

unsigned int loadCubeMap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int WIDTH, HEIGHT, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char *data =
        stbi_load(faces[i].c_str(), &WIDTH, &HEIGHT, &nrChannels, 0);
    if (data == 0) {
      std::cout << "cannot load " << faces[i] << '\n';
    } else {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, WIDTH, HEIGHT,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
