#ifndef DRAWFIGURE_H
#define DRAWFIGURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Constants.h"
#include "Figure.h"

class DrawFigure {
public:
  DrawFigure() {}

  static void drawCircle(glm::vec3 center, float r, int vertexCount = 100) {
    std::vector<Vertex> v;

    v.push_back({glm::vec3(center.x, center.y, 0.5), glm::vec3(0, 0, 1),
                 glm::vec2(0.5)});

    for (int i = 0; i < vertexCount + 1; i++) {
      float theta = i * 2 * M_PI / (float)vertexCount;
      v.push_back(
          {glm::vec3(center.x + r / 2.0f * cos(theta),
                     center.y + r / 2.0f * sin(theta), center.z),
           glm::vec3(0, 0, 1),
           glm::vec2(0.5 - cos(theta) / 2.0f, 0.5 - sin(theta) / 2.0f)});
    }

    Figure circle(GL_TRIANGLE_FAN, true, true, v);
    circle.draw();
  }

  static void drawPlane(glm::vec3 center, glm::vec2 length,
                        bool isFlip = false) {
    std::vector<Vertex> v = {
        // top right
        {glm::vec3(length.x / 2.0f + center.x, length.y / 2.0f + center.y,
                   0.0f + center.z),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},

        // bottom right
        {glm::vec3(length.x / 2.0f + center.x, -length.y / 2.0f + center.y,
                   0.0f + center.z),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},

        // bottom left
        {glm::vec3(-length.x / 2.0f + center.x, -length.y / 2.0f + center.y,
                   0.0f + center.z),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},

        // top left
        {glm::vec3(-length.x / 2.0f + center.x, length.y / 2.0f + center.y,
                   0.0f + center.z),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)}};

    std::vector<GLuint> i = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    if (isFlip) {
      for (Vertex &vertex : v) {
        vertex.normal = glm::vec3(0, 0, -1);
      }
    }

    Figure plane(GL_TRIANGLES, true, true, v, i);
    plane.draw();
  }

  static void drawCube(glm::vec3 center, glm::vec3 length,
                       bool useNormal = true, bool useTexture = true) {
    std::vector<Vertex> v = {
        // positions          // normals           // texture coords
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},

        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},

        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y - length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x + length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z + length.z / 2.0f),
         glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(center.x - length.x / 2.0f, center.y + length.y / 2.0f,
                   center.z - length.z / 2.0f),
         glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}};

    Figure cube(GL_TRIANGLES, useNormal, useTexture, v);
    cube.draw();
  }
};

#endif
