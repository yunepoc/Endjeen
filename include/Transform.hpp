#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ej {

class Transform {
  public:
    glm::vec3 forward();
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rot);
    void setScale(glm::vec3 scale);
    glm::mat4 getModelMatrix();
    glm::vec3 getPosition();
    void translate(glm::vec3 translation);
  private:
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::mat4(1.0f);
    glm::vec3 scale = glm::vec3(1.0f);
};

}
