#pragma once

#include <Game/Placing.hpp>
#include <Game/Terrain.hpp>
#include <System.hpp>

namespace ej {

class Building;
class Worker;
class Game: public System {
  public:
    Game();
    void addBuilding(std::unique_ptr<Building> building);
    void addWorker(std::unique_ptr<Worker> worker);
    Worker* getAvailableWorker(std::string name);
    std::vector<std::unique_ptr<Building>>& getBuildings() { return buildings; }
    void load();
    void receive(SystemMsg& msg);
    void render();
    void shutdown() {}
    void update(double delta);
  private:
    Terrain terrain;
    Placing placing;
    std::vector<std::unique_ptr<Building>> buildings;
    std::vector<std::unique_ptr<Worker>> workers;
};

}
