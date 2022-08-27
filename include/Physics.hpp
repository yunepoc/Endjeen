#pragma once

#include <glm/glm.hpp>
#include <System.hpp>

namespace ej {

class Physics;
class PhysicsBox {
  friend Physics;
  public:
    ~PhysicsBox();
    void setPosition(glm::vec3 position);
    void* getUserData();
    void setUserData(void* data);
    void* getHandle() { return handle; }
  private:
    void* handle = nullptr;
    void* userData;
};

class Physics: public System {
  friend PhysicsBox;
  public:
    ~Physics();
    void createPhysicsBox(PhysicsBox &box, glm::vec3 size);
    std::vector<PhysicsBox*> currentColliders(PhysicsBox &box);
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
