#include <Window.hpp>

#include <GLFW/glfw3.h>

#define WIN static_cast<GLFWwindow*>(handle)

namespace ej {

Window::Window(unsigned width, unsigned height, std::string title) {
  if (!glfwInit())
    throw 0;;
  GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window)
    throw 0;
  handle = static_cast<void*>(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
}

Window::~Window() {
  glfwTerminate();
}
    
bool Window::isOpen() { return !glfwWindowShouldClose(WIN); }
void Window::pollEvents() { glfwPollEvents(); }
void Window::swapBuffers() { glfwSwapBuffers(WIN); }

}
