#include <Game/Game.hpp>

#include <App.hpp>
#include <Game/GameContent.hpp>

namespace ej {

Game::Game(): placing(*this) {}

void Game::addBuilding(std::unique_ptr<Building> building) {
  buildings.push_back(std::move(building));
}

void Game::load() {
  terrain.load();
}

void Game::receive(SystemMsg& msg) {
  if (msg.getSystem() == "UI" && msg.getMsg() == "build") {
    assert(msg.numString() == 1);
    placing.create(msg.getString(0));
  } else if (msg.getSystem() == "input" && msg.getMsg() == "click") {
    placing.click();
  } else if (msg.getSystem() == "input" && msg.getMsg() == "key") {
    if (msg.getInt(0) == Input::Key::R)
      placing.rotate();
  }
}

void Game::render() {
  terrain.render();
  for (auto& building: buildings)
    building->render();
  placing.render();
}

void Game::update() {
  App::instance().getUI().setDebugLabel("# buildings", std::to_string(buildings.size()));
  placing.update();
}

}
