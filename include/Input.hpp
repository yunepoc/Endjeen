#pragma once

namespace ej {

class Window;
class Input {
  public:
    enum class Key {
      // NOTE: Need to add a conversion entry in Input.cpp when adding a key here.
      A, D, S, W,
      Left, Right, Up, Down,
      Add, Subtract,
    };
    static bool keyDown(Key key);
};

}
