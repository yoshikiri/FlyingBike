#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <iostream>
#include <GL/glew.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned int loadTexture(std::string filename, bool useAlpha) {
  stbi_set_flip_vertically_on_load(true);

  int WIDTH, HEIGHT, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &WIDTH, &HEIGHT, &nrChannels, STBI_rgb_alpha);
  if (data == 0) {
    std::cout << "cannot load " << filename << '\n';
  }

  unsigned int textureID = 0;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (useAlpha == false)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  if (useAlpha == true)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return textureID;
}

unsigned int loadCubeMap(std::vector<std::string> faces) {
  stbi_set_flip_vertically_on_load(true);

  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int WIDTH, HEIGHT, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char *data =
        stbi_load(faces[i].c_str(), &WIDTH, &HEIGHT, &nrChannels, 0);
    if (data == 0) {
      std::cout << "cannot load " << faces[i] << '\n';
    } else {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, WIDTH, HEIGHT,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return textureID;
}


#endif