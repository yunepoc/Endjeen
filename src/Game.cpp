#include <Game.hpp>

#include <Transform.hpp>

namespace ej {
  void Game::init(std::string root) {
    window = std::unique_ptr<Window>(new ej::Window(800, 600, "Endjeen"));

    resourceMgr.init(root);
    renderer.load();
  }

  void Game::run() {

    ResRenderable &triangle = resourceMgr.get<ResRenderable>("triangle");
    ResShader &shader = resourceMgr.get<ResShader>("default.shader");

    ResRenderable &plane = resourceMgr.get<ResRenderable>("plane4x4x1.std");
    Transform transform;
    transform.setPosition({0.0f, 0.0f, 0.0f});

    // Grid
    ResRenderable &grid = resourceMgr.get<ResRenderable>("plane1000x1000.std");
    ResShader &gridShader = resourceMgr.get<ResShader>("grid.shader");
    Transform gridTransform;
    gridTransform.setPosition({-500.0f, 0.01f, 500.0f});

    renderer.setWireframeMode(false);
    while (window->isOpen()) {
      camera.update();
      renderer.renderBefore();
      renderer.render(plane, shader, transform, camera);
      renderer.render(grid, gridShader, gridTransform, camera);
      window->swapBuffers();
      window->pollEvents();
    }
  }

  void Game::stop() {
    renderer.shutdown();
  }
}
