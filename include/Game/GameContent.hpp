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

class GameObject {
  public:
    virtual ~GameObject() {};
};

class GameContent;
class Building : public GameObject {
  friend GameContent;
  public:
    Building();
    Building* create();
    ResTexture& getIcon() { return icon; }
    std::string getName() { return name; }
    PhysicsBox& getPhysicsBox() { return box; }
    Transform& getTransform() { return transform; }
    void setTransform(Transform& transform) { this->transform = transform; }
    void render();
    // Set the building as "invalid". This is used to change the material
    // to display something saying the current building is invalid (e.g.
    // pending building on invalid position).
    void setInvalid(bool invalid);
    // Same for "valid" (e.g. pending building on valid position).
    void setValid(bool valid);
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
