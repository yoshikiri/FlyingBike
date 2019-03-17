#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <irrKlang/irrKlang.h>

#include "Play.h"
#include "Result.h"
#include "Title.h"
#include "Window.h"
#include "textureManager.h"

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
  // State *state = new Play(window.getWindow(), 1);
  State *state = new Title(window.getWindow());

  auto soundEngine = std::make_unique<irrklang::ISoundEngine *>(
      irrklang::createIrrKlangDevice());

  (*soundEngine)->play2D("resource/sound/game_maoudamashii_4_field08.ogg", true);
  // State *state = new Result(window.getWindow(), 10.323f, true, 1);
  // State *state = new Result(window.getWindow(), 110.323f, false, 1);

  //--------------------------------------------------------------------------//
  // main loop
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    State *next = state->update();
    if (next != state)
      state = next;

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

    window.swapBuffers();
  }

  delete state;

  return 0;
}
