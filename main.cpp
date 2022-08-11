#include <iostream>
#include <Window.hpp>

int main() {
  ej::Window window(800, 600, "Endjeen");

  while (window.isOpen()) {
    window.swapBuffers();
    window.pollEvents();
  }

  return 0;
}
