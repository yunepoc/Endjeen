#include <Transform.hpp>

#include <glm/gtx/transform.hpp>

namespace ej {

void Transform::setPosition(glm::vec3 position) {
  this->position = position;
}

void Transform::setRotation(glm::vec3 rot) {
  glm::vec3 rotrads = glm::radians(rot);
  rotation = glm::quat(rotrads);
}

void Transform::setScale(glm::vec3 scale) {
  this->scale = scale;
}

glm::vec3 Transform::forward() {
  return glm::vec3(getModelMatrix() * glm::vec4(0,0,-1.0,0));
}

glm::mat4 Transform::getModelMatrix() {
  glm::mat4 mscale = glm::scale(glm::mat4(1.0), scale);
  glm::mat4 mrotation = glm::toMat4(rotation);
  glm::mat4 mposition = glm::translate(glm::mat4(1.0f), position);
  return mposition * mrotation * mscale;
}

glm::vec3 Transform::getPosition() {
  return position;
}

void Transform::translate(glm::vec3 translation) {
  position += translation;
}

}
