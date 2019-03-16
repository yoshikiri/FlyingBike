#ifndef STATE_H
#define STATE_H

#include <memory>

struct GLFWwindow;

class State{
public:
  State(GLFWwindow *window) : window(window){}

  virtual ~State(){}

  virtual State *update() = 0;
  // virtual std::unique_ptr<State> update() = 0;

  GLFWwindow *window;
};

#endif