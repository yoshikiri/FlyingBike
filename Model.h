#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace std {
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T> class allocator;
template <class T, class Allocator> class vector;
}


class Model {
public:
  Model(char *path) { loadModel(path); }

  void draw(Shader shader);

private:
  std::vector<Mesh> meshes;
  std::string directory;
  std::vector<Texture> textures_loaded;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);
};

#endif
