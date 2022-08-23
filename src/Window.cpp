#include <Window.hpp>

#include <App.hpp>
#include <Debug.hpp>
#include <GLFW/glfw3.h>

#define WIN static_cast<GLFWwindow*>(handle)

namespace ej {

static void resizeCallback(GLFWwindow* window, int width, int height) {
  SystemMsg msg("window", "resize");
  msg.addInt(width);
  msg.addInt(height);
  System::send(msg);
}

Window::Window(unsigned width, unsigned height) {
  if (!glfwInit())
    ERROR("Cannot create window");
  GLFWwindow *window = glfwCreateWindow(width, height, "", NULL, NULL);
  if (!window)
    ERROR("Cannot create window");
  handle = static_cast<void*>(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
}

Window::~Window() {
  glfwTerminate();
}

void Window::load() {
  App::instance().getUI().addDebugBoolean("FPS limit", &limitFPS);
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

void Window::update() {
  glfwSwapInterval(limitFPS);
}

void Window::receive(SystemMsg& msg) {
  if (msg.getSystem() == "game" && msg.getMsg() == "loaded") {
    std::string name = App::instance().getGame().getName();
    glfwSetWindowTitle(WIN, name.c_str());
  }
}

}
