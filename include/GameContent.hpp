#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <Material.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <Resource.hpp>
#include <Transform.hpp>
#include <vector>

namespace ej {

class GameContent;
class Building {
  friend GameContent;
  public:
    Building* create();
    std::string getName() { return name; }
    void render();
    void setTilePosition(glm::vec2 tile);
  private:
    std::string name;
    glm::vec2 size;
    ResRenderable renderable;
    Material material;
    Transform transform;
    void load(nlohmann::json &json);
};

class GameContent {
  public:
    Building *createBuilding(std::string name);
    void load();
  private:
    std::vector<std::unique_ptr<Building>> buildings;
};

}
