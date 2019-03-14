#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <random>

// #include "Camera.h"
// #include "Constants.h"
// #include "Container.h"
// #include "DrawFigure.h"
// #include "Goal.h"
// #include "Model.h"
// #include "Play.h"
// #include "Player.h"
#include "Shader.h"
// #include "State.h"
#include "Window.h"
#include "textureManager.h"
#include "Play.h"
// #include "PerlinNoise.h"
// #include "ppm.h"

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
  // create shaders
  // Shader lampShader("../lampVertexShader.vert", "../lampFragmentShader.frag");

  // object shader
  // Shader lightingShader("../directionalLight.vert",
  // "../directionalLight.frag");
  // Shader lightingShader("../pointLight.vert", "../pointLight.frag");
  // Shader lightingShader("../spotLights.vert", "../spotLights.frag");
  // Shader lightingShader("../multipleLight.vert", "../multipleLight.frag");
  //
  // Shader skyboxShader("../skyboxVertexShader.vert",
  //                     "../skyboxFragmentShader.frag");

  //--------------------------------------------------------------------------//
  // load textures
  unsigned int diffuseMapContainer =
      loadTexture("resource/container2.png", true);
  // unsigned int diffuseMapWall = loadTextureM("resource/brick-wall.png",
  // true);
  unsigned int diffuseMapFloor = loadTexture("resource/wood-texture.png", true);
  unsigned int diffuseMapPlayer = loadTexture("resource/poly.png", true);
  // unsigned int diffuseMapFloor = loadTextureM("resource/wood_grain.png",
  // true); unsigned int specularMap = loadTextureM("resource/wall.jpg", false);
  unsigned int specularMap =
      loadTexture("resource/container2_specular.png", true);
  // lightingShader.setInt("material.diffuse", 0);
  // lightingShader.setInt("material.specular", 1);

  //--------------------------------------------------------------------------//
  // create camera (perspective)
  // Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
  //               glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT, true);
  // Camera camera(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
  //               glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT, true);

  // glm::vec3 playerPosition(0, 0, 0.5);
  // glm::vec3 playerVelocity(0);
  // glm::vec3 playerAcceleration(0);
  // Player player(glm::vec3(3, -0.5, 0.5), diffuseMapPlayer, specularMap);

  // glm::vec3 containerPositions[] = {
  //     glm::vec3(0.0f, 0.0f, 0.5f),
  //     glm::vec3(2.5f, 1.5f, 0.5f),
  //     glm::vec3(-1.0f, 1.0f, 0.5f),
  //     glm::vec3(-2.0f, -2.0f, 0.5f),
  // };
  // glm::vec3 goalPositions[] = {
  //     glm::vec3(-1.0f, 0.0f, 0.5f),
  //     glm::vec3(2.0f, 1.5f, 0.5f),
  //     glm::vec3(-3.0f, 1.0f, 0.5f),
  //     glm::vec3(-2.0f, -3.0f, 0.5f),
  // };
  // std::vector<Container> containers = {
  //     {containerPositions[0], glm::vec3(1.0f), diffuseMapContainer,
  //      specularMap},
  //     {containerPositions[1], glm::vec3(1.0f), diffuseMapContainer,
  //      specularMap},
  //     {containerPositions[2], glm::vec3(1.0f), diffuseMapContainer,
  //      specularMap},
  //     {containerPositions[3], glm::vec3(1.0f), diffuseMapContainer,
  //      specularMap},
  // };
  //
  // std::vector<Goal> goals = {
  //     {goalPositions[0], 0.5f},
  //     {goalPositions[1], 0.5f},
  //     {goalPositions[2], 0.5f},
  //     {goalPositions[3], 0.5f},
  // };

  // glm::vec3 target(-2, -2, 0.5);

  unsigned int tex[] = {diffuseMapPlayer, specularMap,     diffuseMapContainer,
                        specularMap,      diffuseMapFloor, specularMap};

  State *state = new Play(window.getWindow(), glm::vec3(3, -0.5, 0.5), tex);
  //--------------------------------------------------------------------------//
  // main loop
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    State *next = state->update();


    //------------------------------------------------------------------------//
    // draw light
    // double radius = 1;
    // glm::vec3 pos(radius * cos(glfwGetTime()), radius * sin(glfwGetTime()),
    //               -3.0);
    // glm::vec3 pos(1.0, 1.0, -3.0);
    // glm::vec3 lampPos(1.2f, 1.0f, 1.0f);
    // glm::vec3 lampPos(-0.6f, 0.6f, 0.6f);
    // glm::mat4 lampModel(glm::translate(glm::mat4(1.0f), lampPos));
    // glm::mat4 lampView = camera.getView();
    // glm::mat4 lampProjection = camera.getProjection();
    // lampShader.use();
    // lampShader.setMat4("model", lampModel);
    // lampShader.setMat4("view", lampView);
    // lampShader.setMat4("projection", lampProjection);
    //
    // // for (int i = 0; i < 4; i++) {
    // //   lampModel = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
    // //   lampShader.setMat4("model", lampModel);
    // //   DrawFigure::drawCube(glm::vec3(0), glm::vec3(0.5), false, false);
    // // }
    //
    // // target = glm::vec3(2 * sin(glfwGetTime()), 2 * cos(glfwGetTime()), 0.5);
    // // DrawFigure::drawCube(target, glm::vec3(1), false, false);
    //
    // //------------------------------------------------------------------------//
    // // set attribute for object shader
    // glm::mat4 model(1.0);
    // glm::mat4 view = camera.getView();
    // glm::mat4 projection = camera.getProjection();
    //
    // lightingShader.use();
    // lightingShader.setMat4("model", model);
    // lightingShader.setMat4("view", view);
    // lightingShader.setMat4("projection", projection);
    // //
    // lightingShader.setVec3("viewPos", camera.getPosition());
    // //
    // lightingShader.setFloat("material.shininess", 32.0f);
    // //
    // // lightingShader.setVec3("light.position", lampPos);
    // // lightingShader.setVec3("light.direction", glm::vec3(cos(glfwGetTime()),
    // // sin(glfwGetTime()), 0) - lampPos);
    // // lightingShader.setVec3("light.direction", glm::vec3(0.0f, 0.0f, -0.3f));
    // // lightingShader.setVec3("light.direction", glm::vec3(0.0f)-lampPos);
    // // lightingShader.setVec3("light.direction", glm::vec3(0.2f, 0.2f, -0.3f));
    // // lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(50.f)));
    // // lightingShader.setFloat("light.outerCutOff",
    // // glm::cos(glm::radians(60.f)));
    //
    // // lightingShader.setVec3("light.ambient", glm::vec3(0.4f, 0.1f, 0.1f));
    // // lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    // // lightingShader.setVec3("light.diffuse", glm::vec3(0.8f, 0.2f, 0.2f));
    // // lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    // // lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    // //
    // // // set attribute when the object shader uses point light
    // // lightingShader.setFloat("light.constant", 1.0f);
    // // lightingShader.setFloat("light.linear", 0.09f);
    // // lightingShader.setFloat("light.quadratic", 0.032f);
    //
    // // directional light
    // lightingShader.setVec3("dirLight.direction", 0.2f, 0.2f, -0.3f);
    // // lightingShader.setVec3("dirLight.direction", 0.0f, 0.0f, -0.1f);
    // lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    // lightingShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    // lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // // point light 1
    // lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
    // lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    // lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    // lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("pointLights[0].constant", 1.0f);
    // lightingShader.setFloat("pointLights[0].linear", 0.09);
    // lightingShader.setFloat("pointLights[0].quadratic", 0.032);
    // // point light 2
    // lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
    // lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    // lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    // lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("pointLights[1].constant", 1.0f);
    // lightingShader.setFloat("pointLights[1].linear", 0.09);
    // lightingShader.setFloat("pointLights[1].quadratic", 0.032);
    // // point light 3
    // lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
    // lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    // lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    // lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("pointLights[2].constant", 1.0f);
    // lightingShader.setFloat("pointLights[2].linear", 0.09);
    // lightingShader.setFloat("pointLights[2].quadratic", 0.032);
    // // point light 4
    // lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
    // lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    // lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    // lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("pointLights[3].constant", 1.0f);
    // lightingShader.setFloat("pointLights[3].linear", 0.09);
    // lightingShader.setFloat("pointLights[3].quadratic", 0.032);
    // // spotLights
    // lightingShader.setVec3("spotLights[0].position",
    //                        goals[0].center + glm::vec3(0, 0, 1));
    // lightingShader.setVec3("spotLights[0].direction", glm::vec3(0, 0, -1));
    // lightingShader.setVec3("spotLights[0].ambient", goals[0].color);
    // lightingShader.setVec3("spotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    // lightingShader.setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("spotLights[0].constant", 1.0f);
    // lightingShader.setFloat("spotLights[0].linear", 0.09);
    // lightingShader.setFloat("spotLights[0].quadratic", 0.032);
    // lightingShader.setFloat("spotLights[0].cutOff",
    //                         glm::cos(glm::radians(12.5f)));
    // lightingShader.setFloat("spotLights[0].outerCutOff",
    //                         glm::cos(glm::radians(15.0f)));
    //
    // lightingShader.setVec3("spotLights[1].position",
    //                        goals[1].center + glm::vec3(0, 0, 1));
    // lightingShader.setVec3("spotLights[1].direction", glm::vec3(0, 0, -1));
    // lightingShader.setVec3("spotLights[1].ambient", goals[1].color);
    // lightingShader.setVec3("spotLights[1].diffuse", 1.0f, 1.0f, 1.0f);
    // lightingShader.setVec3("spotLights[1].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("spotLights[1].constant", 1.0f);
    // lightingShader.setFloat("spotLights[1].linear", 0.09);
    // lightingShader.setFloat("spotLights[1].quadratic", 0.032);
    // lightingShader.setFloat("spotLights[1].cutOff",
    //                         glm::cos(glm::radians(12.5f)));
    // lightingShader.setFloat("spotLights[1].outerCutOff",
    //                         glm::cos(glm::radians(15.0f)));
    //
    // lightingShader.setVec3("spotLights[2].position",
    //                        goals[2].center + glm::vec3(0, 0, 1));
    // lightingShader.setVec3("spotLights[2].direction", glm::vec3(0, 0, -1));
    // lightingShader.setVec3("spotLights[2].ambient", goals[2].color);
    // lightingShader.setVec3("spotLights[2].diffuse", 1.0f, 1.0f, 1.0f);
    // lightingShader.setVec3("spotLights[2].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("spotLights[2].constant", 1.0f);
    // lightingShader.setFloat("spotLights[2].linear", 0.09);
    // lightingShader.setFloat("spotLights[2].quadratic", 0.032);
    // lightingShader.setFloat("spotLights[2].cutOff",
    //                         glm::cos(glm::radians(12.5f)));
    // lightingShader.setFloat("spotLights[2].outerCutOff",
    //                         glm::cos(glm::radians(15.0f)));
    //
    // lightingShader.setVec3("spotLights[3].position",
    //                        goals[3].center + glm::vec3(0, 0, 1));
    // lightingShader.setVec3("spotLights[3].direction", glm::vec3(0, 0, -1));
    // lightingShader.setVec3("spotLights[3].ambient", goals[3].color);
    // lightingShader.setVec3("spotLights[3].diffuse", 1.0f, 1.0f, 1.0f);
    // lightingShader.setVec3("spotLights[3].specular", 1.0f, 1.0f, 1.0f);
    // lightingShader.setFloat("spotLights[3].constant", 1.0f);
    // lightingShader.setFloat("spotLights[3].linear", 0.09);
    // lightingShader.setFloat("spotLights[3].quadratic", 0.032);
    // lightingShader.setFloat("spotLights[3].cutOff",
    //                         glm::cos(glm::radians(12.5f)));
    // lightingShader.setFloat("spotLights[3].outerCutOff",
    //                         glm::cos(glm::radians(15.0f)));
    //
    // // std::unique_ptr<std::vector<Goal, std::allocator<Goal>>> goals;
    // // glm::vec3 c = (*goals)[0].color;
    //
    // //------------------------------------------------------------------------//
    // // draw objects
    // for (Container c : containers) {
    //   c.draw();
    // }
    //
    // model = glm::mat4(1.0f);
    // lightingShader.setMat4("model", model);
    //
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, diffuseMapFloor);
    // for (int y = 0; y < 10; y++) {
    //   for (int x = 0; x < 10; x++) {
    //     DrawFigure::drawPlane(glm::vec3(-4 + x, -4 + y, 0), glm::vec2(1, 1));
    //   }
    // }
    //
    // //------------------------------------------------------------------------//
    // // draw player
    // if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    //   player.velocity.x += 0.001;
    // if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    //   player.velocity.x -= 0.001;
    // if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    //   player.velocity.y += 0.001;
    // if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    //   player.velocity.y -= 0.001;
    //
    // // playerPosition += playerVelocity;
    // // glActiveTexture(GL_TEXTURE0);
    // // glBindTexture(GL_TEXTURE_2D, diffuseMapPlayer);
    // // DrawFigure::drawCube(playerPosition, glm::vec3(1));
    //
    // // player.seek(target);
    //
    // //------------------------------------------------------------------------//
    // // check if player's collision
    // for (Container c : containers)
    //   player.checkCollision(c);
    //
    // for (Goal &g : goals)
    //   player.checkGoal(g);
    //
    // player.update();
    // player.draw();

    // std::cout << player.position.x << '\n';

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
