#pragma once

#include <glm/glm.hpp>
#include <System.hpp>
#include <Transform.hpp>

namespace ej {

class Camera : public System {
  public:
    Camera();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    // Takes an {x,y} screen point and return its world point and direction relative
    // to camera. This is used for mouse click ray casting.
    std::pair<glm::vec3, glm::vec3> screenToRay(glm::uvec2 coordinates);
    void update(float delta);
  private:
    float zOffset = 6.0f;
    Transform transform;

    void load() {}
    void receive(SystemMsg& msg);
    void shutdown() {}
};

}
