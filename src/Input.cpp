#include <Input.hpp>

#include <App.hpp>
#include <Debug.hpp>
#include <GLFW/glfw3.h>

namespace ej {

static int keyToGLFWKey(Input::Key key) {
  switch (key) {
    case Input::Key::A: return GLFW_KEY_A;
    case Input::Key::D: return GLFW_KEY_D;
    case Input::Key::R: return GLFW_KEY_R;
    case Input::Key::S: return GLFW_KEY_S;
    case Input::Key::W: return GLFW_KEY_W;
    case Input::Key::Left: return GLFW_KEY_LEFT;
    case Input::Key::Right: return GLFW_KEY_RIGHT;
    case Input::Key::Up: return GLFW_KEY_UP;
    case Input::Key::Down: return GLFW_KEY_DOWN;
    case Input::Key::Add: return GLFW_KEY_KP_ADD;
    case Input::Key::Subtract: return GLFW_KEY_KP_SUBTRACT;
    case Input::Key::ShiftLeft: return GLFW_KEY_LEFT_SHIFT;
    default: ERROR("Input: Unknown key");
  }
}

static Input::Key GLFWKeyToKey(int key) {
  switch (key) {
    case GLFW_KEY_A: return Input::Key::A;
    case GLFW_KEY_D: return Input::Key::D;
    case GLFW_KEY_R: return Input::Key::R;
    case GLFW_KEY_S: return Input::Key::S;
    case GLFW_KEY_W: return Input::Key::W;
    case GLFW_KEY_LEFT: return Input::Key::Left;
    case GLFW_KEY_RIGHT: return Input::Key::Right;
    case GLFW_KEY_UP: return Input::Key::Up;
    case GLFW_KEY_DOWN: return Input::Key::Down;
    case GLFW_KEY_KP_ADD: return Input::Key::Add;
    case GLFW_KEY_KP_SUBTRACT: return Input::Key::Subtract;
    case GLFW_KEY_LEFT_SHIFT: return Input::Key::ShiftLeft;
    default: ERROR("Input: Unknown key");
  }
}

glm::vec2 Input::getMousePosition() {
  double x, y;
  GLFWwindow *window = static_cast<GLFWwindow*>(App::instance().getWindow().getHandle());
  glfwGetCursorPos(window, &x, &y);
  return {x, y};
}

bool Input::keyDown(Key key) {
  void* handle = App::instance().getWindow().getHandle();
  GLFWwindow* window = static_cast<GLFWwindow*>(handle);
  int GLFWKey = keyToGLFWKey(key);
  return glfwGetKey(window, GLFWKey) == GLFW_PRESS;
}

bool Input::mouseLeftDown() {
  void* handle = App::instance().getWindow().getHandle();
  GLFWwindow* window = static_cast<GLFWwindow*>(handle);
  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  return state == GLFW_PRESS;
}

bool Input::mouseLeftUp() {
  void* handle = App::instance().getWindow().getHandle();
  GLFWwindow* window = static_cast<GLFWwindow*>(handle);
  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  return state == GLFW_RELEASE;
}

bool Input::mouseRightDown() {
  void* handle = App::instance().getWindow().getHandle();
  GLFWwindow* window = static_cast<GLFWwindow*>(handle);
  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
  return state == GLFW_PRESS;
}

bool Input::mouseRightUp() {
  void* handle = App::instance().getWindow().getHandle();
  GLFWwindow* window = static_cast<GLFWwindow*>(handle);
  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
  return state == GLFW_RELEASE;
}

static void scrollCallback(GLFWwindow* window, double x, double y) {
  SystemMsg msg("input", "scroll");
  msg.addInt(x);
  msg.addInt(y);
  System::send(msg);
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
  if (UI::wantsToCaptureMouse())
    return;
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    SystemMsg msg("input", "click");
    System::send(msg);
  }
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    SystemMsg msg("input", "key");
    msg.addInt(GLFWKeyToKey(key));
    System::send(msg);
  }
}

void Input::load() {
  void* handle = App::instance().getWindow().getHandle();
  glfwSetScrollCallback((GLFWwindow*)handle, scrollCallback);
  glfwSetMouseButtonCallback((GLFWwindow*)handle, mouseCallback);
  glfwSetKeyCallback((GLFWwindow*)handle, keyCallback);
}

}
