#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
  Window(int width = 640, int height = 480, const char *title = "Tutorial")
      : window(glfwCreateWindow(width, height, title, NULL, NULL)),
        scale(100.0f), location{0, 0} {
    if (window == NULL) {
      std::cerr << "cannot create window." << '\n';
      exit(1);
    }

    // windowを対象にする
    glfwMakeContextCurrent(window);

    // GLEWを初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      std::cerr << "cannot init GLEW" << '\n';
      exit(1);
    }

    glfwSwapInterval(1);

    // windowに自身を示すポインタを登録
    glfwSetWindowUserPointer(window, this);

    // set callback function for resize event
    glfwSetWindowSizeCallback(window, resize);

    glfwSetScrollCallback(window, wheel);

    resize(window, width, height);

  }

  virtual ~Window() { glfwDestroyWindow(window); }

  int shouldClose() const {
    return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
  }

  void swapBuffers() {
    glfwSwapBuffers(window);
    // glfwWaitEvents();
    glfwPollEvents();

    // マウスカーソルの位置を取得
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE) {
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
      location[1] = -(static_cast<GLfloat>(y) * 2.0f / size[1] - 1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE) {
      location[0] -= 2.0f / size[0];
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE) {
      location[0] += 2.0f / size[0];
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE) {
      location[1] -= 2.0f / size[1];
    } else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE) {
      location[1] += 2.0f / size[1];
    }
  }

  GLfloat getAspect() const { return aspect; }

  static void resize(GLFWwindow *const window, int width, int height) {
    glViewport(0, 0, width, height);

    Window *const instance(
        static_cast<Window *>(glfwGetWindowUserPointer(window)));

    if (instance != NULL) {
      instance->aspect =
          static_cast<GLfloat>(width) / static_cast<GLfloat>(height);

      instance->size[0] = static_cast<GLfloat>(width);
      instance->size[1] = static_cast<GLfloat>(height);
    }
  }

  static void wheel(GLFWwindow *const window, double x, double y) {
    Window *const instance(
        static_cast<Window *>(glfwGetWindowUserPointer(window)));
    if (instance != NULL) {
      instance->scale += static_cast<GLfloat>(y) * 2;
    }
  }

  const GLfloat *getSize() const { return size; }

  const GLfloat getScale() const { return scale; }

  const GLfloat *getLocation() const { return location; }

  GLFWwindow* getWindow() { return window;}


private:
  GLFWwindow *const window;
  GLfloat aspect;
  GLfloat size[2];
  GLfloat scale;
  GLfloat location[2];
};
