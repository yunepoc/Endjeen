#pragma once

#include <Camera.hpp>
#include <memory>
#include <Resource.hpp>
#include <Renderer.hpp>
#include <Window.hpp>

namespace ej {

class Game {
  public:
    Game(const Game&) = delete;
    Game& operator=(const Game &) = delete;
    Game(Game &&) = delete;
    Game& operator=(Game &&) = delete;
    static auto& instance(){
        static Game game;
        return game;
    }

    Window& getWindow() { return *window; }
    ResourceManager& getResourceMgr() { return resourceMgr; }
    void init(std::string root);
    void run();
    void stop();
  private:
    Game() {};
    ResourceManager resourceMgr;
    Renderer renderer;
    std::unique_ptr<Window> window;
    Camera camera;
};

}
