#include <Transform.hpp>

#include <glm/gtx/transform.hpp>

namespace ej {

void Transform::setPosition(glm::vec3 position) {
  this->position = position;
}

glm::mat4 Transform::getModelMatrix() {
  return glm::translate(glm::mat4(1.0f), position);
}

}
