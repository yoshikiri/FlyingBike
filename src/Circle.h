#ifndef CIRCLE_H
#define CIRCLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Circle {
public:
  Circle(glm::vec3 center, float radius) : center(center), radius(radius) {}

  glm::vec3 center;
  float radius;
};

#endif