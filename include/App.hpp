#pragma once

#include <Camera.hpp>
#include <Game.hpp>
#include <Input.hpp>
#include <memory>
#include <Physics.hpp>
#include <Resource.hpp>
#include <Renderer.hpp>
#include <UI.hpp>
#include <Window.hpp>

namespace ej {

class App {
  public:
    App(const App&) = delete;
    App& operator=(const App &) = delete;
    App(App &&) = delete;
    App& operator=(App &&) = delete;
    static auto& instance(){
        static App app;
        return app;
    }

    Camera& getCamera() { return camera; }
    Game& getGame() { return game; }
    Physics& getPhysics() { return physics; }
    Renderer& getRenderer() { return renderer; }
    UI& getUI() { return ui; }
    Window& getWindow() { return *window; }
    ResourceManager& getResourceMgr() { return resourceMgr; }
    void init(std::string root);
    void run();
    void stop();
  private:
    App() {};
    Game game;
    Input input;
    Physics physics;
    ResourceManager resourceMgr;
    Renderer renderer;
    UI ui;
    std::unique_ptr<Window> window;
    Camera camera;

    float lastFrame = 0.0;
    float delta = 0.0;

    bool showGrid = false;
};

}
