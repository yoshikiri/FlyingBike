#ifndef CONTAINTER_H
#define CONTAINTER_H

#include "AABB.h"

class Container : public AABB {
public:
  Container(glm::vec3 center, glm::vec3 length, unsigned int diffuseTextureID,
            unsigned int specularTextureID);

  void update();

  void draw() const;

private:
  unsigned int diffuseTextureID;
  unsigned int specularTextureID;
};

#endif