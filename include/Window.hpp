#pragma once

#include <glm/glm.hpp>
#include <string>

namespace ej {

class Window {
  public:
    Window(unsigned width, unsigned height, std::string title);
    ~Window();

    void* getHandle() { return handle; }
    glm::uvec2 getSize();
    bool isOpen();
    void pollEvents();
    void swapBuffers();
  private:
    void* handle;
};

}
