#pragma once

#include <glm/glm.hpp>
#include <System.hpp>

namespace ej {

class Physics;
class PhysicsBox {
  friend Physics;
  public:
    void setPosition(glm::vec3 position);
  private:
    void* handle = nullptr;
};

class Physics: public System {
  public:
    ~Physics();
    void createPhysicsBox(PhysicsBox &box, glm::vec3 size);
    void load();
    std::pair<PhysicsBox*, glm::vec3> raycast(glm::vec3 point, glm::vec3 direction);
    void render();
    void shutdown();
    void update(float delta);
  private:
    struct Handle;
    Handle *handle;
    bool debug = false;
};

}
