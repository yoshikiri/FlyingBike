#ifndef AABB_H
#define AABB_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Circle.h"

class AABB {
public:
  AABB(glm::vec3 center, glm::vec3 length)
      : center(center), length(length), max(center + length / 2.0f),
        min(center - length / 2.0f){};

  bool isIntersected(const AABB &obj) const {
    if (min.x > obj.max.x)
      return false;
    if (max.x < obj.min.x)
      return false;
    if (min.y > obj.max.y)
      return false;
    if (max.y < obj.min.y)
      return false;
    if (min.z > obj.max.z)
      return false;
    if (max.z < obj.min.z)
      return false;

    return true;
  }

  bool isIntersected(const Circle &obj) {
    float dist = 0.0f;

    if (min.y < obj.center.y && obj.center.y < max.y) {
      if (min.x < obj.center.x && obj.center.x < max.x)
        dist = 0.0f;
      else
        dist = std::max(obj.center.x - max.x, min.x - obj.center.x);
    } else if (min.x < obj.center.x && obj.center.x < max.x) {
      if (min.y < obj.center.y && obj.center.y < max.y)
        dist = 0.0f;
      else
        dist = std::max(obj.center.y - max.y, min.y - obj.center.y);
    } else
      return false;

    if (dist < obj.radius)
      return true;

    return false;
  }

  glm::vec3 center;
  const glm::vec3 length;
  glm::vec3 max;
  glm::vec3 min;
};

#endif