#ifndef FIGURE_H
#define FIGURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Figure {
public:
  Figure(GLenum mode, unsigned int vertexCount, const float vertices[],
         const unsigned int indexCount = 0, const unsigned int indices[] = NULL)
      : mode(mode), vertexCount(vertexCount), indexCount(indexCount) {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * vertexCount, vertices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount,
                 indices, GL_STATIC_DRAW);

    /* attribute変数の取り出し方を指定 */
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
  }

  ~Figure() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
  }

  void draw() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);

    if (indexCount == 0) {
      glDrawArrays(mode, 0, vertexCount);
    } else {
      glDrawElements(mode, indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
  }

private:
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  const GLenum mode;
  const unsigned int vertexCount;
  const unsigned int indexCount;
};

#endif
