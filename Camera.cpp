#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float width,
               float height, bool isPerspective)
    : position(position), target(target), up(up), width(width), height(height) {

  fovy = 40.0f;

  if (isPerspective)
    projection =
        glm::perspective(glm::radians(fovy), width / height, 0.1f, 100.0f);
  view = glm::lookAt(position, target, up);
}

void Camera::scrollCallback(GLFWwindow *window, double xoffset,
                            double yoffset) {
  if (fovy >= 1.0f && fovy <= 45.0f)
    fovy -= yoffset;
  else if (fovy <= 1.0f)
    fovy = 1.0f;
  else if (fovy >= 45.0f)
    fovy = 45.0f;
}

glm::vec3 Camera::getPosition() const { return position; }

glm::mat4 Camera::getProjection() const { return projection; }

glm::mat4 Camera::getView() const { return view; }
