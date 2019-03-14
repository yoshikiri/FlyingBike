#ifndef STATE_H
#define STATE_H

struct GLFWwindow;

class State{
public:
  State(GLFWwindow *window) : window(window){}

  virtual State *update() = 0;

  GLFWwindow *window;
};

#endif