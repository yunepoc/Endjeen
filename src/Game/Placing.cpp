#include <Game/Placing.hpp>

#include <App.hpp>
#include <Game/Game.hpp>
#include <Game/GameContent.hpp>
#include <Input.hpp>

namespace ej {

void Placing::create(std::string name) {
  Building *b = App::instance().getGameContent().createBuilding(name);
  b->setTilePosition({0, 0});
  current.reset(b);
}

void Placing::render() {
  if (current)
    current->render();
}

void Placing::update() {
  if (!current)
    return;
  glm::vec2 mousePos = Input::getMousePosition();
  auto ray = App::instance().getCamera().screenToRay(mousePos);
  glm::vec3 start = ray.first;
  glm::vec3 dir = ray.second;

  auto res = App::instance().getPhysics().raycast(start, dir);
  if (res.first) {
    int tilex = std::floor(res.second.x);
    int tiley = std::floor(res.second.z*-1);
    current->setTilePosition({tilex, tiley});
  }
  if (Input::mouseRightDown())
    current = nullptr;
  else if (Input::mouseLeftDown()) {
    std::unique_ptr<Building> b = std::move(current);
    if (Input::keyDown(Input::Key::ShiftLeft))
      create(b->getName());
    game.addBuilding(std::move(b));
  }
}

}
