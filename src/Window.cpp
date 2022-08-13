#include <Window.hpp>

#include <Debug.hpp>
#include <GLFW/glfw3.h>

#define WIN static_cast<GLFWwindow*>(handle)

namespace ej {

Window::Window(unsigned width, unsigned height, std::string title) {
  if (!glfwInit())
    ERROR("Cannot create window");
  GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window)
    ERROR("Cannot create window");
  handle = static_cast<void*>(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
}

Window::~Window() {
  glfwTerminate();
}

glm::uvec2 Window::getSize() {
  int width;
  int height;
  glfwGetWindowSize(WIN, &width, &height);
  return {width, height};
}

bool Window::isOpen() { return !glfwWindowShouldClose(WIN); }
void Window::pollEvents() { glfwPollEvents(); }
void Window::swapBuffers() { glfwSwapBuffers(WIN); }

}
