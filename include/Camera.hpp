#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ej {

class Camera {
  public:
    Camera();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    void update(float delta);
  private:
    float zOffset = 6.0f;
    glm::vec3 position = glm::vec3(0.0f);
};

}
