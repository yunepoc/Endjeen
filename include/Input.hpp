#pragma once

#include <glm/glm.hpp>
#include <System.hpp>

namespace ej {

class Input : public System {
  public:
    enum Key {
      // NOTE: Need to add a conversion entry in Input.cpp when adding a key here.
      A, D, R, S, W,
      Left, Right, Up, Down,
      Add, Subtract,
      ShiftLeft,
    };
    static glm::vec2 getMousePosition();
    static bool keyDown(Key key);
    static bool mouseLeftDown();
    static bool mouseLeftUp();
    static bool mouseRightDown();
    static bool mouseRightUp();
    void load();
    void shutdown() {};
};

}
