#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
  Window(int width = 800, int height = 600, const char *title = "Window")
      : window(glfwCreateWindow(width, height, title, NULL, NULL)) {
    if (window == NULL) {
      std::cerr << "cannot create window." << '\n';
      exit(1);
    }

    glfwMakeContextCurrent(window);

    // init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      std::cerr << "cannot init GLEW" << '\n';
      exit(1);
    }

    glfwSwapInterval(1);

    // set self pointer to window
    glfwSetWindowUserPointer(window, this);
  }

  virtual ~Window() { glfwDestroyWindow(window); }

  int shouldClose() const {
    return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
  }

  void swapBuffers() {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GLFWwindow *getWindow() const { return window; }

private:
  GLFWwindow *const window;
};
