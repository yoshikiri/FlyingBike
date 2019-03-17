#include "Goal.h"

Goal::Goal(glm::vec3 center, float radius)
    : Circle(center, radius), color(glm::vec3(1.0f, 0.0f, 0.0f)),
      isClear(false) {}

void Goal::checkClear() {
  color = glm::vec3(0.0f, 1.0f, 0.0f);
  isClear = true;
}