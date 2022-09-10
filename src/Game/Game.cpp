#include <Game/Game.hpp>

#include <App.hpp>
#include <Game/GameContent.hpp>

namespace ej {

Game::Game(): placing(*this) {}

void Game::addBuilding(std::unique_ptr<Building> building) {
  building->load();
  buildings.push_back(std::move(building));
}

void Game::addWorker(std::unique_ptr<Worker> worker) {
  workers.push_back(std::move(worker));
}

Worker* Game::getAvailableWorker(std::string name) {
  for (auto &worker: workers) {
    if (worker->getName() != name)
      continue;
    if (worker->isAvailable())
      return &*worker;
  }
  Worker *worker = App::instance().getGameContent().createWorker(name);
  worker->load();
  std::unique_ptr<Worker> w(worker);
  addWorker(std::move(w));
  return worker;
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
  for (auto& worker: workers)
    worker->render();
  placing.render();
}

void Game::update(double delta) {
  App::instance().getUI().setDebugLabel("Buildings", std::to_string(buildings.size()));
  App::instance().getUI().setDebugLabel("Workers", std::to_string(workers.size()));
  placing.update();
  for (auto& building: buildings)
    building->update(delta);
  for (auto& worker: workers) {
    worker->update(delta);
  }
}

}
