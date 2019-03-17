#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float width,
               float height, bool isPerspective)
    : position(position), target(target), up(up), width(width), height(height) {

  fovy = 300.0f;

  if (isPerspective)
    projection =
        glm::perspective(glm::radians(fovy), width / height, 0.1f, 100.0f);
  else
    projection = glm::ortho(-width, width, -height, height, 0.1f, 1000.0f);
  view = glm::lookAt(position, target, up);
}

glm::vec3 Camera::getPosition() const { return position; }

glm::mat4 Camera::getProjection() const { return projection; }

glm::mat4 Camera::getView() const { return view; }
