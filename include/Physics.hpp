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
    void createPhysicsBox(PhysicsBox &box, glm::vec2 size);
    void load();
    void render();
    void shutdown();
  private:
    struct Handle;
    Handle *handle;
    bool debug = false;
};

}
