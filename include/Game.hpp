#pragma once

#include <memory>
#include <Resource.hpp>
#include <Renderer.hpp>
#include <Window.hpp>

namespace ej {

class Game {
  public:
    void init();
    void run();
    void stop();
  private:
    ResourceManager resourceMgr;
    Renderer renderer;
    std::unique_ptr<Window> window;
};

}
