#pragma once

#include <memory>
#include <string>

namespace ej {

class Building;
class Game;
class Placing {
  public:
    Placing(Game& game): game(game) {};
    void click();
    void create(std::string name);
    void render();
    void rotate();
    void update();
  private:
    Game& game;
    std::unique_ptr<Building> current;
    bool pendingClick = false;
    bool pendingRotation = false;
    void reset();
};

}
