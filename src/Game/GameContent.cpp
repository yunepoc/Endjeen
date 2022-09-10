#include <Game/GameContent.hpp>

#include <App.hpp>
#include <fstream>
#include <Script.hpp>

namespace ej {

void GameObject::load() {
  if (hasScript)
    Script::load(script, *this);
}

void GameObject::render() {
  if (!isTemplate && visible) {
    Renderer &renderer = App::instance().getRenderer();
    renderer.render(renderable, material, transform);
  }
}

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

Building* Building::closest(std::string name) {
  assert(!isTemplate && "Unexpected call on template object");
  auto &buildings = App::instance().getGame().getBuildings();
  glm::vec2 position = {transform.getPosition().x, transform.getPosition().z};
  Building* closest = nullptr;
  float distanceMin = 9999999999;
  for (auto &building: buildings) {
    if (building->getName() != name)
      continue;
    glm::vec3 p = building->getTransform().getPosition();
    glm::vec2 targetPosition = {p.x, p.z};
    float distance = glm::distance(position, targetPosition);
    if (distance < distanceMin) {
      closest = &*building;
      distanceMin = distance;
    }
  }
  return closest;
}

void Building::setInvalid(bool invalid) {
  assert(!isTemplate && "Unexpected call on template object");
  material.setUniformBool("invalid", invalid);
}

void Building::setValid(bool valid) {
  assert(!isTemplate && "Unexpected call on template object");
  material.setUniformBool("valid", valid);
}

void Building::setTilePosition(glm::vec2 tile) {
  assert(!isTemplate && "Unexpected call on template object");
  float x = tile.x + size.x/2.0f;
  float y = -tile.y - size.y/2.0f;
  transform.setPosition({x, 0.0, y});
  box.setPosition({x, 0.0, y});
}

void Building::loadJSON(nlohmann::json &json) {
  name = json["name"];
  size = { json["size"][0], json["size"][1] };
  renderable = App::instance().getResourceMgr().get<ResRenderable>(json["model"]);
  ResTexture &texture = App::instance().getResourceMgr().get<ResTexture>(json["texture"]);
  ResShader &shader = App::instance().getResourceMgr().get<ResShader>("building.shader");
  if (json.contains("script")) {
    hasScript = true;
    script = App::instance().getResourceMgr().get<ResScript>(json["script"]);
  }
  material = Material(shader, {&texture});
  icon = App::instance().getResourceMgr().get<ResTexture>(json["icon512"]);
}

void Building::update(double delta) {
  assert(!isTemplate && "Unexpected call on template object");
  if (hasScript)
    Script::update(script, delta);
}

Worker* Worker::create() {
  Worker *worker = new Worker(*this);
  worker->isTemplate = false;
  worker->setVisible(false);
  return worker;
}

void Worker::loadJSON(nlohmann::json &json) {
  name = json["name"];
  if (json.contains("script")) {
    hasScript = true;
    script = App::instance().getResourceMgr().get<ResScript>(json["script"]);
  }
  renderable = App::instance().getResourceMgr().get<ResRenderable>(json["model"]);
  ResShader &shader = App::instance().getResourceMgr().get<ResShader>("default.shader");
  ResTexture &texture = App::instance().getResourceMgr().get<ResTexture>(json["texture"]);
  material = Material(shader, {&texture});
}

void Worker::setAvailable(bool available) {
  assert(!isTemplate && "Unexpected call on template object");
  this->available = available;
  if (available) {
    building = nullptr;
    target = nullptr;
  }
}

void Worker::setBuilding(Building *building) {
  assert(!isTemplate && "Unexpected call on template object");
  this->building = building;
  transform.setPosition(building->getTransform().getPosition());
}

void Worker::update(double delta) {
  assert(!isTemplate && "Unexpected call on template object");
  Script::update(script, delta);
  // If the worker has a target, move toward it
  if (!target || !building)
    return;
  // Returning to building
  if (target == building) {
    glm::vec3 bpos = building->getTransform().getPosition();
    glm::vec3 wpos = transform.getPosition();
    // Compute remaining distance
    float distRem = glm::length(bpos - wpos);
    // Compute step distance
    glm::vec3 dir = bpos - wpos;
    dir = glm::normalize(dir);
    dir *= delta;
    float distStep = glm::length(dir);
    // Update position
    if (distRem < distStep)
      transform.setPosition(building->getTransform().getPosition());
    else
      transform.translate({dir.x, 0, dir.z});
    // Check if target reached
    glm::vec3 newWpos = transform.getPosition();
    float dist = glm::length(bpos - newWpos);
    if (dist < 0.1)
      target = nullptr;
  // Moving to target
  } else {
    glm::vec3 tpos = target->getTransform().getPosition();
    glm::vec3 bpos = building->getTransform().getPosition();
    glm::vec3 wpos = transform.getPosition();
    // Compute remaining distance
    float distRem = glm::length(tpos - wpos);
    // Compute step distance
    glm::vec3 dir = tpos - bpos;
    dir = glm::normalize(dir);
    dir *= delta;
    float distStep = glm::length(dir);
    // Update position
    if (distRem < distStep)
      transform.setPosition(target->getTransform().getPosition());
    else
      transform.translate({dir.x, 0, dir.z});
    // Check if target reached
    glm::vec3 newWpos = transform.getPosition();
    float dist = glm::length(tpos - newWpos);
    if (dist < 0.1)
      target = nullptr;
  }
}

Building* GameContent::createBuilding(std::string name) {
  for (auto &b: buildings)
    if (b->getName() == name)
      return b->create();
  ERROR("Invalid building name");
}

Worker* GameContent::createWorker(std::string name) {
  for (auto &w: workers)
    if (w->getName() == name)
      return w->create();
  ERROR("Invalid worker name");
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
      building->loadJSON(data);
      buildings.push_back(std::move(building));
    }
  // Load workers
  std::string pathWorkers = resMgr.getFilePath("logic/workers/");
  if (!std::filesystem::is_directory(pathBuildings))
    ERROR("Invalid path");
  for (const auto &file : std::filesystem::directory_iterator(pathWorkers))
    if (file.is_regular_file() && file.path().extension() == ".json") {
      std::unique_ptr<Worker> worker = std::make_unique<Worker>();
      std::ifstream stream(file.path());
      nlohmann::json data = nlohmann::json::parse(stream);
      worker->loadJSON(data);
      workers.push_back(std::move(worker));
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
