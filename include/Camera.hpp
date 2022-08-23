#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <System.hpp>
#include <Transform.hpp>

namespace ej {

class Camera : public System {
  public:
    Camera();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    void update(float delta);
  private:
    float zOffset = 6.0f;
    Transform transform;

    void load() {}
    void receive(SystemMsg& msg);
    void shutdown() {}
};

}
