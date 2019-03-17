#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <string>

#include "AABB.h"
#include "Goal.h"

class Player : public AABB {
public:
  Player();
  Player(glm::vec3 center, unsigned int diffuseTextureID,
         unsigned int specularTextureID);

  void update();
  void seek(const glm::vec3 target);
  void checkCollision(const AABB &obj);
  bool checkGoal(Goal &obj);
  void applyForce(const glm::vec3 force);
  void draw();

private:
  glm::vec3 velocity;
  glm::vec3 acceleration;

  float maxSpeed;
  float maxForce;
  unsigned int diffuseTextureID;
  unsigned int specularTextureID;
};

#endif