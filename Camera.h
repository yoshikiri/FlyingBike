#ifndef CAMERA_H
#define CAMERA_H

// #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
public:
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float width, float height, bool isPerspective = true);

  void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

  glm::vec3 getPosition() const;
  glm::mat4 getProjection() const;
  glm::mat4 getView() const;

private:
  glm::vec3 position, target, up;
  const float width, height;
  glm::mat4 projection;
  glm::mat4 view;
  bool isPerspective;
  float fovy;
};

#endif