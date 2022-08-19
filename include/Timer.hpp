#pragma once

#include <GLFW/glfw3.h>

namespace ej {

namespace Timer {

double getTime() {
  return glfwGetTime();
}

}

}
