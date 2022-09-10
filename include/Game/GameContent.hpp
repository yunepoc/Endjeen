#pragma once

#include <glm/glm.hpp>
#include <Material.hpp>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <Physics.hpp>
#include <Resource.hpp>
#include <Transform.hpp>
#include <vector>

namespace ej {

class GameContent;
class GameObject {
  public:
    virtual void load();
    std::string getName() { return name; }
    Transform& getTransform() { return transform; }
    virtual void loadJSON(nlohmann::json &json) = 0;
    void render();
    void setTransform(Transform& transform) { this->transform = transform; }
    // Show/hide object model
    void setVisible(bool visible) { this->visible = visible; }
  protected:
    std::string name;
    Transform transform;
    bool isTemplate = true;
    bool visible = true;
    ResRenderable renderable;
    Material material;
    ResScript script;
    bool hasScript = false;
};

class Building : public GameObject {
  public:
    Building();
    Building* create();
    // Return closest building with given name
    Building* closest(std::string name);
    ResTexture& getIcon() { return icon; }
    PhysicsBox& getPhysicsBox() { return box; }
    void loadJSON(nlohmann::json &json);
    // Set the building as "invalid". This is used to change the material
    // to display something saying the current building is invalid (e.g.
    // pending building on invalid position).
    void setInvalid(bool invalid);
    void setTilePosition(glm::vec2 tile);
    // Same for "valid" (e.g. pending building on valid position).
    void setValid(bool valid);
    void update(double delta);
  private:
    glm::vec2 size;
    PhysicsBox box;
    ResTexture icon;
};

class Worker : public GameObject {
  public:
    Worker* create();
    Building* getBuilding() { return building; }
    // Return true if the worker is done with its work
    bool getDone() { return done; }
    // Return true if the worker has a target building
    bool hasTarget() { return target != nullptr; }
    // Return true if the worker is available. An available worker is a worker
    // with no current work to do, so it can be used by any building.
    bool isAvailable() { return available; }
    void loadJSON(nlohmann::json &json);
    // Set the worker as available/unavailable
    void setAvailable(bool available);
    // Set the worker associated building
    void setBuilding(Building *building);
    // Set the worker done status
    void setDone(bool done) { this->done = done; }
    // Set the worker target. If both target and building are defined, the worker
    // is moved from building to target
    void setTarget(Building* building) { target = building; }
    void update(double delta);
  private:
    bool available = true;
    bool done = false;
    Building *building = nullptr;
    Building *target = nullptr;
};

class GameContent {
  public:
    Building *createBuilding(std::string name);
    Worker *createWorker(std::string name);
    std::vector<std::unique_ptr<Building>>& getBuildings() { return buildings; }
    std::map<std::string, unsigned>& getResources() { return resources; }
    void resourceAdd(std::string name, unsigned value);
    bool resourceHas(std::string name);
    void resourceSub(std::string name, unsigned value);
    std::string getName() { return name; }
    void load();
  private:
    std::string name;
    std::vector<std::unique_ptr<Building>> buildings;
    std::vector<std::unique_ptr<Worker>> workers;
    void loadInfos(nlohmann::json &json);
    std::map<std::string, unsigned> resources;
};

}
