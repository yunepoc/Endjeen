#pragma once

#include <System.hpp>

namespace ej {

class UI: public System {
  public:
    void addDebugBoolean(std::string name, bool* value);
    void load();
    void render();
    void shutdown() {};
  private:
    void debugUI();
    void gameUI();

    // Values showed in debug window
    std::vector<std::pair<std::string, bool*>> debugBooleans;
};

}
