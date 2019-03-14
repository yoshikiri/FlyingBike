#include "Player.h"

#include "DrawFigure.h"
// #include "textureManager.h"

namespace {
const float MAX_SPEED = 0.05f;
const float MAX_FORCE = 0.001f;
const float LENGTH = 1.0f;
} // namespace

Player::Player()
    : AABB(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(LENGTH)), velocity(0.0f),
      acceleration(0.0f), maxSpeed(MAX_SPEED), maxForce(MAX_FORCE) {}

Player::Player(glm::vec3 center, unsigned int diffuseTextureID,
               unsigned int specularTextureID)
    : AABB(center, glm::vec3(LENGTH)), velocity(0.0f), acceleration(0.0f),
      maxSpeed(MAX_SPEED), maxForce(MAX_FORCE),
      diffuseTextureID(diffuseTextureID), specularTextureID(specularTextureID) {
}

void Player::update() {
  velocity += acceleration;
  if (glm::length(velocity) > maxSpeed) {
    velocity = glm::normalize(velocity);
    velocity *= maxSpeed;
  }

  center += velocity;
  acceleration *= 0;

  min = center - length / 2.0f;
  max = center + length / 2.0f;
}

void Player::seek(glm::vec3 target) {
  glm::vec3 desired = target - center;
  desired = glm::normalize(desired);
  desired *= maxSpeed;
  glm::vec3 steer = desired - velocity;
  if (glm::length(steer) > maxForce) {
    steer = glm::normalize(steer);
    steer *= maxForce;
  }

  applyForce(steer);
}

void Player::checkCollision(const AABB &obj) {
  if (this->isIntersected(obj) == false)
    return;

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // speed down
  // if (glm::length(velocity) > maxSpeed) {
  velocity = glm::normalize(velocity);
  velocity *= (maxSpeed / 5.0f);
  // }
}

void Player::checkGoal(Goal &obj) {
  if (this->isIntersected(obj)) {
    // return true;
    // obj.color = glm::vec3(0.0f, 1.0f, 0.0f);
    obj.checkClear();
  }
  // return false;
}

void Player::applyForce(glm::vec3 force) { acceleration += force / 1.0f; }

void Player::draw() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, specularTextureID);
  DrawFigure::drawCube(center, glm::vec3(1.0));
}
