#include <Game.hpp>

namespace ej {
  void Game::init(std::string root) {
    window = std::unique_ptr<Window>(new ej::Window(800, 600, "Endjeen"));

    resourceMgr.init(root);
    renderer.load();
  }

  void Game::run() {

    ResRenderable &triangle = resourceMgr.get<ResRenderable>("triangle");
    ResShader &shader = resourceMgr.get<ResShader>("default.shader");

    while (window->isOpen()) {
      camera.update();
      renderer.renderBefore();
      renderer.render(triangle, shader, camera);
      window->swapBuffers();
      window->pollEvents();
    }
  }

  void Game::stop() {
    renderer.shutdown();
  }
}
