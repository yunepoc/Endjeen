#pragma once

#include <System.hpp>

namespace ej {

class UI: public System {
  public:
    // TODO: instead of passing a boolean ptr, we should store the value here
    // and send a message when a value is changed.
    void addDebugBoolean(std::string name, bool* value);
    void setDebugLabel(std::string name, std::string value);
    void load();
    void render();
    void shutdown() {};
  private:
    void debugUI();
    void gameUI();

    // Values showed in debug window
    std::vector<std::pair<std::string, bool*>> debugBooleans;
    std::vector<std::pair<std::string, std::string>> debugLabels;
};

}
