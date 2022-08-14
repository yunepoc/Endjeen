#pragma once

#include <glm/glm.hpp>

namespace ej {

class Transform {
  public:
    void setPosition(glm::vec3 position);
    glm::mat4 getModelMatrix();
  private:
    glm::vec3 position = glm::vec3(0.0f);;
};

}
