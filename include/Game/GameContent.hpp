#pragma once

#include <glm/glm.hpp>
#include <Material.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <Physics.hpp>
#include <Resource.hpp>
#include <Transform.hpp>
#include <vector>

namespace ej {

class GameContent;
class Building {
  friend GameContent;
  public:
    Building* create();
    ResTexture& getIcon() { return icon; }
    std::string getName() { return name; }
    void render();
    void setTilePosition(glm::vec2 tile);
  private:
    bool isTemplate = true;
    std::string name;
    glm::vec2 size;
    ResRenderable renderable;
    Material material;
    PhysicsBox box;
    Transform transform;
    ResTexture icon;
    void load(nlohmann::json &json);
};

class GameContent {
  public:
    Building *createBuilding(std::string name);
    std::vector<std::unique_ptr<Building>>& getBuildings() { return buildings; }
    std::string getName() { return name; }
    void load();
  private:
    std::string name;
    std::vector<std::unique_ptr<Building>> buildings;
    void loadInfos(nlohmann::json &json);
};

}
