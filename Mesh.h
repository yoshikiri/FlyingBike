#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Shader.h"

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
