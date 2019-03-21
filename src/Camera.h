#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float width,
         float height, bool isPerspective = true);

  glm::vec3 getPosition() const;
  glm::mat4 getProjection() const;
  glm::mat4 getView() const;

private:
  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 up;
  const float width;
  const float height;
  const bool isPerspective;
  glm::mat4 projection;
  glm::mat4 view;
  float fovy;
};

#endif