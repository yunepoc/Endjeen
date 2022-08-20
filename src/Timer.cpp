#include <Timer.hpp>

#include <GLFW/glfw3.h>

namespace ej {

double Timer::getTime() {
  return glfwGetTime();
}

}
