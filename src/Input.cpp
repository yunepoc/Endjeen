#include <Input.hpp>

#include <App.hpp>
#include <Debug.hpp>
#include <GLFW/glfw3.h>

namespace ej {

static int keyToGLFWKey(Input::Key key) {
  switch (key) {
    case Input::Key::A: return GLFW_KEY_A;
    case Input::Key::D: return GLFW_KEY_D;
    case Input::Key::S: return GLFW_KEY_S;
    case Input::Key::W: return GLFW_KEY_W;
    case Input::Key::Left: return GLFW_KEY_LEFT;
    case Input::Key::Right: return GLFW_KEY_RIGHT;
    case Input::Key::Up: return GLFW_KEY_UP;
    case Input::Key::Down: return GLFW_KEY_DOWN;
    case Input::Key::Add: return GLFW_KEY_KP_ADD;
    case Input::Key::Subtract: return GLFW_KEY_KP_SUBTRACT;
    default: ERROR("Input: Unknown key");
  }
}

bool Input::keyDown(Key key) {
  void* handle = App::instance().getWindow().getHandle();
  GLFWwindow* window = static_cast<GLFWwindow*>(handle);
  int GLFWKey = keyToGLFWKey(key);
  return glfwGetKey(window, GLFWKey) == GLFW_PRESS;
}

static void scrollCallback(GLFWwindow* window, double x, double y) {
  SystemMsg msg("input", "scroll");
  msg.addInt(x);
  msg.addInt(y);
  System::send(msg);
}

void Input::load() {
  void* handle = App::instance().getWindow().getHandle();
  glfwSetScrollCallback((GLFWwindow*)handle, scrollCallback);
}

}
