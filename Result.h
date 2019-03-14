#ifndef RESULT_H
#define RESULT_H

#include "State.h"

class Result : public State{
public:
  Result(GLFWwindow *window);

  State *update() override;
};

#endif