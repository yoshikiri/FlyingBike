#include "Container.h"
#include "DrawFigure.h"

Container::Container(glm::vec3 center, glm::vec3 length,
                     unsigned int diffuseTextureID,
                     unsigned int specularTextureID)
    : AABB(center, length), diffuseTextureID(diffuseTextureID),
      specularTextureID(specularTextureID) {}

void Container::update() {}

void Container::draw() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, specularTextureID);
  DrawFigure::drawCube(center, glm::vec3(1.0f));
}