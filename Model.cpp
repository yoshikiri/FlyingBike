#include "Model.h"

#include <iostream>
#include <vector>
#include <stb_image.h>


unsigned int loadTexture(std::string filename, bool rgbaFlag = false) {
  int WIDTH, HEIGHT, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &WIDTH, &HEIGHT, &nrChannels, STBI_rgb_alpha);
  if (data == 0) {
    std::cout << "cannot load " << filename << '\n';
  }

  unsigned int texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (rgbaFlag == false)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  if (rgbaFlag == true)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}

void Model::draw(Shader shader) {
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].draw(shader);
}

void Model::loadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<VertexForMesh> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  // process vertices
  for (int i = 0; i < mesh->mNumVertices; i++) {
    VertexForMesh vertex;

    glm::vec3 vposition;
    vposition.x = mesh->mVertices[i].x;
    vposition.y = mesh->mVertices[i].y;
    vposition.z = mesh->mVertices[i].z;
    vertex.position = vposition;

    glm::vec3 vnormal;
    vnormal.x = mesh->mNormals[i].x;
    vnormal.y = mesh->mNormals[i].y;
    vnormal.z = mesh->mNormals[i].z;
    vertex.normal = vnormal;

    if (mesh->mTextureCoords[0]) {
      glm::vec2 vTexture;
      vTexture.x = mesh->mTextureCoords[0][i].x;
      vTexture.y = mesh->mTextureCoords[0][i].y;
      vertex.textureCoords = vTexture;
    } else {
      vertex.textureCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  // process indices
  for (int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // process materials
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName) {
  std::vector<Texture> textures;

  for (int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    bool skip = false;
    for (int j = 0; j < textures_loaded.size(); j++) {
      // skip loading texture if it has been loaded
      if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      std::string file_path = directory + '/' + str.C_Str();

      Texture texture;
      texture.id = loadTexture(file_path, true);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
    }
  }

  return textures;
}