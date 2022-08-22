#include <Game.hpp>

#include <App.hpp>
#include <fstream>

namespace ej {

Building* Building::create() {
  return new Building(*this);
}

void Building::render() {
  Renderer &renderer = App::instance().getRenderer();
  renderer.render(renderable, material, transform);
}

void Building::setTilePosition(glm::vec2 tile) {
  float x = tile.x + size.x/2.0f;
  float y = -tile.y - size.y/2.0f;
  transform.setPosition({x, 0.0, y});
}

void Building::load(nlohmann::json &json) {
  name = json["name"];
  size = { json["size"][0], json["size"][1] };
  renderable = App::instance().getResourceMgr().get<ResRenderable>(json["model"]);
  ResTexture &texture = App::instance().getResourceMgr().get<ResTexture>(json["texture"]);
  ResShader &shader = App::instance().getResourceMgr().get<ResShader>("default.shader");
  material = Material(shader, {&texture});
  icon = App::instance().getResourceMgr().get<ResTexture>(json["icon512"]);
  setTilePosition({0, 0});
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
  SystemMsg msg("game","loaded");
  System::send(msg);
}

void Game::loadInfos(nlohmann::json &json) {
  name = json["name"];
}

}
