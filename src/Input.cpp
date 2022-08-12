#include <Input.hpp>

#include <Game.hpp>
#include <GLFW/glfw3.h>

namespace ej {

static int keyToGLFWKey(Input::Key key) {
  switch (key) {
    case Input::Key::A: return GLFW_KEY_A;
    case Input::Key::D: return GLFW_KEY_D;
    case Input::Key::S: return GLFW_KEY_S;
    case Input::Key::W: return GLFW_KEY_W;
    default: throw 0;
  }
}

bool Input::keyDown(Key key) {
  void* handle = Game::instance().getWindow().getHandle();
  GLFWwindow* window = static_cast<GLFWwindow*>(handle);
  int GLFWKey = keyToGLFWKey(key);
  return glfwGetKey(window, GLFWKey) == GLFW_PRESS;
}

}
