#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// #include "Shader.h"
#include "Window.h"
#include "textureManager.h"
#include "Play.h"
#include "Result.h"

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
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
  // glClearColor(0.9f, 0.9f, 0.9f, 0.0f);

  // enable culling
  // glFrontFace(GL_CCW);
  // glCullFace(GL_BACK);
  // glEnable(GL_CULL_FACE);

  // load texture flip flop vertically
  // stbi_set_flip_vertically_on_load(true);

  // enable depth buffer
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  //--------------------------------------------------------------------------//
  // load model
  // char nanosuit_path[] = "resource/nanosuit/nanosuit.obj";
  // Model myModel(nanosuit_path);
  //
  // glm::vec3 pointLightPositions[] = {
  //     glm::vec3(3.0f, 3.0f, 0.2f), glm::vec3(2.3f, -3.3f, 1.5f),
  //     glm::vec3(-1.0f, 2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 2.0f)};

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

  // unsigned int triMap = loadTextureM("resource/wall.jpg", false);
  //
  // std::vector<std::string> faces = {
  //     "resource/skybox/right.jpg", "resource/skybox/left.jpg",
  //     "resource/skybox/top.jpg",   "resource/skybox/bottom.jpg",
  //     "resource/skybox/front.jpg", "resource/skybox/back.jpg"};
  // unsigned int cubeMap = loadCubeMap(faces);

  //--------------------------------------------------------------------------//
  // load textures
  unsigned int diffuseMapContainer =
      loadTexture("resource/container2.png", true);
  // unsigned int diffuseMapWall = loadTextureM("resource/brick-wall.png",
  // true);
  unsigned int diffuseMapFloor = loadTexture("resource/wood-texture.png", true);
  unsigned int diffuseMapPlayer = loadTexture("resource/poly.png", true);
  unsigned int diffuseMapTarget = loadTexture("resource/one.png", true);
  // unsigned int diffuseMapFloor = loadTextureM("resource/wood_grain.png",
  // true); unsigned int specularMap = loadTextureM("resource/wall.jpg", false);
  unsigned int specularMap =
      loadTexture("resource/container2_specular.png", true);

  unsigned int *numbers = new unsigned int[9];
  for(int i=0;i<9;i++){
    char buff[5];
    snprintf(buff, sizeof(buff), "%d", i+1);
    numbers[i] = loadTexture(std::string("resource/number/") + std::string(buff) + ".png", true);
  }
  // lightingShader.setInt("material.diffuse", 0);
  // lightingShader.setInt("material.specular", 1);

  //--------------------------------------------------------------------------//
  unsigned int tex[] = {diffuseMapPlayer, specularMap,     diffuseMapContainer,
                        specularMap,      diffuseMapFloor, diffuseMapTarget};

  State *state = new Play(window.getWindow(), glm::vec3(3, -0.5, 0.5), 0);
  // State *state = new Result(window.getWindow(), 10.323f, true, 1);
  // State *state = new Result(window.getWindow(), 110.323f, false, 1);
  //--------------------------------------------------------------------------//
  // main loop
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    State *next = state->update();
    if(next != state) state = next;


    //------------------------------------------------------------------------//


    // model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0));
    // lightingShader.setMat4("model", model);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
