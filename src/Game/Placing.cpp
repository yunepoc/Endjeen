#include <Game/Placing.hpp>

#include <App.hpp>
#include <Game/Game.hpp>
#include <Game/GameContent.hpp>
#include <Input.hpp>

namespace ej {

void Placing::click() {
  pendingClick = true;
}

void Placing::create(std::string name) {
  Building *b = App::instance().getGameContent().createBuilding(name);
  b->setTilePosition({0, 0});
  current.reset(b);
}

void Placing::render() {
  if (current)
    current->render();
}

void Placing::reset() {
  pendingClick = false;
  pendingRotation = false;
}

void Placing::rotate() {
  pendingRotation = true;
}

void Placing::update() {
  if (!current)
    return;
  // Raycast with terrain to set building position
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
  //
  if (pendingRotation) {
    current->getTransform().rotate({0.0, -90.0, 0.0});
    pendingRotation = false;
  }
  //
  // Check if current position is a valid position
  // TODO: Should be better to use groups for physics objects here
  bool validPosition = true;
  std::vector<PhysicsBox*> colliders = App::instance().getPhysics().currentColliders(current->getPhysicsBox());
  for (auto collider: colliders) {
    GameObject *object = (GameObject*)collider->getUserData();
    Building* building = dynamic_cast<Building*>(object);
    if (building) {
      validPosition = false;
      break;
    }
  }
  current->setValid(validPosition);
  current->setInvalid(!validPosition);
  if (Input::mouseRightDown()) {
    reset();
    current = nullptr;
  }
  else if (pendingClick) {
    pendingClick = false;
    if (validPosition) {
      reset();
      current->setValid(false);
      current->setInvalid(false);
      std::string name = current->getName();
      Transform transform = current->getTransform();
      game.addBuilding(std::move(current));
      if (Input::keyDown(Input::Key::ShiftLeft)) {
        create(name);
        current->setTransform(transform);
      }
    }
  }
}

}
