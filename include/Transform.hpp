#pragma once

#include <glm/glm.hpp>

namespace ej {

class Transform {
  public:
    void setPosition(glm::vec3 position);
    void setScale(glm::vec3 scale);
    glm::mat4 getModelMatrix();
    glm::vec3 getPosition();
  private:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
};

}
