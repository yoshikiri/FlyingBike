#include "Mesh.h"

Mesh::Mesh(std::vector<VertexForMesh> vertices, std::vector<GLuint> indices,
           std::vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 頂点バッファオブジェクトの作成
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertexForMesh) * vertices.size(),
               &vertices[0], GL_STATIC_DRAW);

  // attribute変数がデータを取り出す場所を指定
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexForMesh),
                        (void *)offsetof(VertexForMesh, position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexForMesh),
                        (void *)offsetof(VertexForMesh, normal));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexForMesh),
                        (void *)offsetof(VertexForMesh, textureCoords));

  // 0,1,2番目のattribute変数を有効にする
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // ibo
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               &indices[0], GL_STATIC_DRAW);
}

void Mesh::draw(Shader shader) {
  GLuint diffuseNr = 1;
  GLuint specularNr = 1;

  for (int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);

    shader.setFloat(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
