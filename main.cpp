#include <Debug.hpp>
#include <Game.hpp>

int main() {

  LOG("Sarting game...");
  ej::Game& game = ej::Game::instance();
  game.init("../../../GameContent/");
  game.run();
  game.stop();

  return 0;
}
