#ifndef DRAWFIGURE_H
#define DRAWFIGURE_H

#include <glm/glm.hpp>

#include "Constants.h"
#include "Figure.h"

class DrawFigure {
public:
  DrawFigure() {}

  // static void drawCircle(const glm::vec3 center, const float radius,
  //                        const float vertexCount, glm::vec3 color) {
  //   int n = 8 * vertexCount;
  //   float circleVertex[n];
  //   circleVertex[0] = center.x + radius;
  //   circleVertex[1] = center.y;
  //   circleVertex[2] = center.z;
  //   circleVertex[3] = color.x;
  //   circleVertex[4] = color.y;
  //   circleVertex[5] = color.z;
  //   circleVertex[6] = 0.0f;
  //   circleVertex[7] = 0.0f;
  //
  //   for (int i = 8; i < n; i += 8) {
  //     circleVertex[i] = center.x + radius * cos((i - 8) * 2 * 3.14 /
  //     (float)n); circleVertex[i + 1] =
  //         center.y + radius * sin((i - 8) * 2 * 3.14 / (float)n);
  //     circleVertex[i + 2] = center.z;
  //     circleVertex[i + 3] = color.x;
  //     circleVertex[i + 4] = color.y;
  //     circleVertex[i + 5] = color.z;
  //     circleVertex[i + 6] = 0.0f;
  //     circleVertex[i + 7] = 0.0f;
  //   }
  //
  //   // Figure circle(GL_TRIANGLE_FAN, vertexCount, circleVertex);
  //   // circle.draw();
  // }

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

  // static void drawArrow(glm::vec3 center, glm::vec3 direction) {
  //   glm::vec3 normal(direction.y, -direction.x, 0);
  //   normal = glm::normalize(normal);
  //   glm::vec3 p1 = center + direction;
  //   glm::vec3 p2 = center - normal / 4.0f;
  //   glm::vec3 p3 = center + normal / 4.0f;
  //
  //   const float triangleVertex[] = {
  //       p1.x, p1.y, p1.z, 0.2f, 0.2f, 0.5f, 0.0f, 0.0f,  // top
  //       p2.x, p2.y, p2.z, 0.1f, 0.1f, 0.1f, 0.0f, 0.0f,  // left
  //       p3.x, p3.y, p3.z, 0.1f, 0.1f, 0.1f, 0.0f, 0.0f}; // right
  //
  //   // Figure triangle(GL_TRIANGLES, 3, triangleVertex);
  //   // triangle.draw();
  // }

  static void drawPlane(glm::vec3 center, glm::vec2 length) {
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

    Figure plane(GL_TRIANGLES, true, true, v, i);
    plane.draw();
  }

  static void drawRectangle(glm::vec3 center, float width, float height) {
    float rectangleVertex[] = {center.x - width / 2,
                               center.y + height / 2,
                               center.z,
                               1.0f,
                               1.0f,
                               1.0f,
                               0.0f,
                               0.0f,
                               center.x + width / 2,
                               center.y + height / 2,
                               center.z,
                               1.0f,
                               1.0f,
                               1.0f,
                               0.0f,
                               0.0f,
                               center.x + width / 2,
                               center.y - height / 2,
                               center.z,
                               1.0f,
                               1.0f,
                               1.0f,
                               0.0f,
                               0.0f,
                               center.x - width / 2,
                               center.y - height / 2,
                               center.z,
                               1.0f,
                               1.0f,
                               1.0f,
                               0.0f,
                               0.0f};

    // Figure rectangle(GL_LINE_LOOP, 4, rectangleVertex);
    // rectangle.draw();
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
