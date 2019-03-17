#ifndef FIGURE_H
#define FIGURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <iterator>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoords;
};

class Figure {
public:
  Figure(GLenum mode, bool useNormal, bool useTexture,
         std::vector<Vertex> vertices, std::vector<GLuint> indices = {})
      : mode(mode), vertices(vertices), indices(indices) {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
                 &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
                 &indices[0], GL_STATIC_DRAW);

    // set how to get attribute
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    // normal
    if (useNormal) {
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void *)(offsetof(Vertex, normal)));
      glEnableVertexAttribArray(1);
    }
    // TexCoord
    if (useTexture) {
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void *)(offsetof(Vertex, textureCoords)));
      glEnableVertexAttribArray(2);
    }
  }

  ~Figure() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
  }

  void draw() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);

    if (indices.size() == 0) {
      glDrawArrays(mode, 0, vertices.size());
    } else {
      glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
  }

private:
  unsigned int vao;
  unsigned int vbo;
  unsigned int ibo;
  const GLenum mode;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<glm::vec2> textureCoords;
};

#endif
