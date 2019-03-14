#ifndef GOAL_H
#define GOAL_H

#include <glm/glm.hpp>

#include "Circle.h"

class Goal : public Circle {
public:
  Goal(glm::vec3 center, float radius);

  void checkClear();

private:
  glm::vec3 color;
  bool isClear;
};

#endif