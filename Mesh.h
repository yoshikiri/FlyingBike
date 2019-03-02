#ifndef MESH_H
#define MESH_H

#include "Shader.h"

namespace std {
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T> class allocator;
template <class T, class Allocator> class vector;
} // namespace std

struct VertexForMesh {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoords;
};

struct Texture {
  GLuint id;
  std::string type;
  std::string path;
};

class Mesh {
public:
  std::vector<VertexForMesh> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<VertexForMesh> vertices, std::vector<GLuint> indices,
       std::vector<Texture> textures);

  void draw(Shader shader);

private:
  GLuint vao, vbo, ibo;

  void setupMesh();
};

#endif
