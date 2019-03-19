#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <irrKlang/irrKlang.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Play.h"
#include "Result.h"
#include "Title.h"
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

  const float WIDTH = 800.0f;
  const float HEIGHT = 600.0f;
  Window window(WIDTH, HEIGHT, "TrackMaker");

  atexit(glfwTerminate);

  // set background color
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // enable depth buffer
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  //--------------------------------------------------------------------------//
  // play BGM
  auto soundEngine = std::make_unique<irrklang::ISoundEngine *>(
      irrklang::createIrrKlangDevice());
  (*soundEngine)
      ->play2D("resource/sound/kadentu_bgm.wav", true);

  // game state pointer
  State *state = new Title(window.getWindow());

  //--------------------------------------------------------------------------//
  // main loop
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    State *next = state->update();
    if (next != state)
      state = next;

    window.swapBuffers();
  }

  delete state;

  return 0;
}
