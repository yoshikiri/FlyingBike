#ifndef TITLE_H
#define TITLE_H

#include "State.h"

class Title : public State{
public:
  Title(GLFWwindow *window);

  State *update() override;
};

#endif