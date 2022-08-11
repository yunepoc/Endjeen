#pragma once

#include <string>

namespace ej {

class Window {
  public:
    Window(unsigned width, unsigned height, std::string title);
    ~Window();

    bool isOpen();
    void pollEvents();
    void swapBuffers();
  private:
    void* handle;
};

}
