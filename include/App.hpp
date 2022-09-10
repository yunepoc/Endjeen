#pragma once

#include <Camera.hpp>
#include <Game/Game.hpp>
#include <Game/GameContent.hpp>
#include <Input.hpp>
#include <memory>
#include <Physics.hpp>
#include <Resource.hpp>
#include <Renderer.hpp>
#include <UI.hpp>
#include <Window.hpp>

namespace ej {

class App : public System {
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
    GameContent& getGameContent() { return gameContent; }
    Physics& getPhysics() { return physics; }
    Renderer& getRenderer() { return renderer; }
    UI& getUI() { return ui; }
    Window& getWindow() { return *window; }
    ResourceManager& getResourceMgr() { return resourceMgr; }
    void init(std::string root);
    void load() {};
    void run();
    void stop();
    void shutdown() {};
  private:
    App() {};
    Game game;
    GameContent gameContent;
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

    void receive(SystemMsg& msg);
    bool quitRequest = false;
};

}
