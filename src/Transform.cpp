#include <Transform.hpp>

#include <glm/gtx/transform.hpp>

namespace ej {

void Transform::setPosition(glm::vec3 position) {
  this->position = position;
}

void Transform::setScale(glm::vec3 scale) {
  this->scale = scale;
}

glm::mat4 Transform::getModelMatrix() {
  glm::mat4 mscale = glm::scale(glm::mat4(1.0), scale);
  glm::mat4 mposition = glm::translate(glm::mat4(1.0f), position);
  return mposition * mscale;
}

glm::vec3 Transform::getPosition() {
  return position;
}

}
