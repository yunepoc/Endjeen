#include <Game.hpp>

#include <App.hpp>
#include <fstream>

namespace ej {

Building* Building::create() {
  Building* instance = new Building(*this);
  App::instance().getPhysics().createPhysicsBox(instance->box, {size.x, 0, size.y});
  instance->isTemplate = false;
  return instance;
}

void Building::render() {
  if (!isTemplate) {
    Renderer &renderer = App::instance().getRenderer();
    renderer.render(renderable, material, transform);
  }
}

void Building::setTilePosition(glm::vec2 tile) {
  assert(!isTemplate && "Cannot set position of a template building");
  float x = tile.x + size.x/2.0f;
  float y = -tile.y - size.y/2.0f;
  transform.setPosition({x, 0.0, y});
  box.setPosition({x, 0.0, y});
}

void Building::load(nlohmann::json &json) {
  name = json["name"];
  size = { json["size"][0], json["size"][1] };
  renderable = App::instance().getResourceMgr().get<ResRenderable>(json["model"]);
  ResTexture &texture = App::instance().getResourceMgr().get<ResTexture>(json["texture"]);
  ResShader &shader = App::instance().getResourceMgr().get<ResShader>("default.shader");
  material = Material(shader, {&texture});
  icon = App::instance().getResourceMgr().get<ResTexture>(json["icon512"]);
}

Building* Game::createBuilding(std::string name) {
  for (auto &b: buildings)
    if (b->getName() == name)
      return b->create();
  ERROR("Invalid building name");
}

void Game::load() {
  ResourceManager& resMgr = App::instance().getResourceMgr();
  // Load game infos
  std::string pathInfos = resMgr.getFilePath("logic/game.json");
  if (!std::filesystem::exists(pathInfos))
    ERROR("Invalid path");
  std::ifstream stream(pathInfos);
  nlohmann::json data = nlohmann::json::parse(stream);
  loadInfos(data);
  // Load buildings
  std::string pathBuildings = resMgr.getFilePath("logic/buildings/");
  if (!std::filesystem::is_directory(pathBuildings))
    ERROR("Invalid path");
  for (const auto &file : std::filesystem::directory_iterator(pathBuildings))
    if (file.is_regular_file() && file.path().extension() == ".json") {
      std::unique_ptr<Building> building = std::make_unique<Building>();
      std::ifstream stream(file.path());
      nlohmann::json data = nlohmann::json::parse(stream);
      building->load(data);
      buildings.push_back(std::move(building));
    }
  //
  terrain.load();
  //
  SystemMsg msg("game","loaded");
  System::send(msg);
}

void Game::receive(SystemMsg& msg) {
  if (msg.getSystem() != "UI" && msg.getMsg() != "build")
    return;
  assert(msg.numString() == 1);
  if (current)
    delete current;
  current = createBuilding(msg.getString(0));
  current->setTilePosition({0, 0});
}

void Game::render() {
  terrain.render();
  for (auto building: placed)
    building->render();
  if (!current)
    return;
  current->render();
}

void Game::shutdown() {
  if (current)
    delete current;
  for (auto building: placed)
    delete building;
}

void Game::update() {
  App::instance().getUI().setDebugLabel("# buildings", std::to_string(placed.size()));
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
    placed.push_back(current);
    if (Input::keyDown(Input::Key::ShiftLeft)) {
      current = createBuilding(current->getName());
    }
    else
      current = nullptr;
  }
}

void Game::loadInfos(nlohmann::json &json) {
  name = json["name"];
}

}
