#include <Game/GameContent.hpp>

#include <App.hpp>
#include <fstream>

namespace ej {

Building::Building() {
  material.setUniformBool("invalid", false);
  material.setUniformBool("valid", false);
}

Building* Building::create() {
  Building* instance = new Building(*this);
  App::instance().getPhysics().createPhysicsBox(instance->box, {size.x - 0.1, 0, size.y - 0.1});
  instance->box.setUserData(this);
  instance->isTemplate = false;
  return instance;
}

void Building::render() {
  if (!isTemplate) {
    Renderer &renderer = App::instance().getRenderer();
    renderer.render(renderable, material, transform);
  }
}

void Building::setInvalid(bool invalid) {
  material.setUniformBool("invalid", invalid);
}

void Building::setValid(bool valid) {
  material.setUniformBool("valid", valid);
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
  ResShader &shader = App::instance().getResourceMgr().get<ResShader>("building.shader");
  material = Material(shader, {&texture});
  icon = App::instance().getResourceMgr().get<ResTexture>(json["icon512"]);
}

Building* GameContent::createBuilding(std::string name) {
  for (auto &b: buildings)
    if (b->getName() == name)
      return b->create();
  ERROR("Invalid building name");
}

void GameContent::load() {
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
  SystemMsg msg("game","loaded");
  System::send(msg);
}

void GameContent::loadInfos(nlohmann::json &json) {
  name = json["name"];
  for (nlohmann::json& res: json["resources"]) {
    std::string r = res.get<std::string>();
    resources[r] = 0;
  }
}

void GameContent::resourceAdd(std::string name, unsigned value) {
  assert(resources.count(name) == 1 && "Invalid resource");
  resources[name] += value;
}

bool GameContent::resourceHas(std::string name) {
  return resources.count(name) == 1;
}

void GameContent::resourceSub(std::string name, unsigned value) {
  assert(resources.count(name) == 1 && "Invalid resource");
  resources[name] -= value;
}

}
