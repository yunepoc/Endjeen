#include <Game.hpp>

int main() {
  ej::Game& game = ej::Game::instance();
  game.init();
  game.run();
  game.stop();
  return 0;
}
