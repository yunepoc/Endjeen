#pragma once

#include <Game/Placing.hpp>
#include <Game/Terrain.hpp>
#include <System.hpp>

namespace ej {

class Building;
class Game: public System {
  public:
    Game();
    void addBuilding(std::unique_ptr<Building> building);
    void load();
    void receive(SystemMsg& msg);
    void render();
    void shutdown() {}
    void update();
  private:
    Terrain terrain;
    Placing placing;
    std::vector<std::unique_ptr<Building>> buildings;
};

}
