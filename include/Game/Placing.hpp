#pragma once

#include <memory>
#include <string>
#include <vector>

namespace ej {

class Building;
class Game;
class Placing {
  public:
    Placing(Game& game): game(game) {};
    void create(std::string name);
    void render();
    void update();
  private:
    Game& game;
    std::unique_ptr<Building> current;
};

}
