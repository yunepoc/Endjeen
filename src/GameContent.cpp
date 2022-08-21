#include <GameContent.hpp>

#include <fstream>
#include <Game.hpp>

namespace ej {

Building* Building::create() {
  return new Building(*this);
}

void Building::render() {
  Renderer &renderer = Game::instance().getRenderer();
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
  renderable = Game::instance().getResourceMgr().get<ResRenderable>(json["model"]);
  ResTexture &texture = Game::instance().getResourceMgr().get<ResTexture>(json["texture"]);
  ResShader &shader = Game::instance().getResourceMgr().get<ResShader>("default.shader");
  material = Material(shader, {&texture});
  setTilePosition({0, 0});
}

Building* GameContent::createBuilding(std::string name) {
  for (auto &b: buildings)
    if (b->getName() == name)
      return b->create();
  ERROR("Invalid building name");
}

void GameContent::load() {
  // Load buildings
  std::string pathBuildings = Game::instance().getResourceMgr().getFilePath("logic/buildings/");
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
}

}
