#pragma once

#include <glm/glm.hpp>
#include <string>
#include <System.hpp>

namespace ej {

class Window : public System {
  public:
    Window(unsigned width, unsigned height);
    ~Window();

    void* getHandle() { return handle; }
    glm::uvec2 getSize();
    bool isOpen();
    void load() {};
    void pollEvents();
    void shutdown() {};
    void swapBuffers();
  private:
    void* handle;
    void receive(SystemMsg& msg);
};

}
