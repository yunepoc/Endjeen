#include <Game.hpp>

#include <Material.hpp>
#include <Terrain.hpp>
#include <Timer.hpp>
#include <Transform.hpp>

namespace ej {
  void Game::init(std::string root) {
    window = std::unique_ptr<Window>(new ej::Window(2560, 1440, "Endjeen"));

    resourceMgr.init(root);
    // The order of the loads is important here
    ui.load();
    renderer.load();
    content.load();
    ui.addDebugBoolean("Show grid", &showGrid);
  }

  void Game::run() {

    Terrain terrain;

    // Grid
    ResRenderable &grid = resourceMgr.get<ResRenderable>("plane1000x1000.std");
    ResShader &gridShader = resourceMgr.get<ResShader>("grid.shader");
    Transform gridTransform;
    gridTransform.setPosition({-500.0f, 0.01f, 500.0f});
    Material gridMaterial(gridShader);

    // Test building
    Building *house = content.createBuilding("house");
    house->setTilePosition({8, 10});

    while (window->isOpen()) {
      // Compute delta
      float frame = Timer::getTime();
      delta = frame - lastFrame;
      lastFrame = frame;

      camera.update(delta);
      renderer.renderBefore();
      terrain.render();
      if (showGrid)
        renderer.render(grid, gridMaterial, gridTransform);
      house->render();
      ui.render();
      window->swapBuffers();
      window->pollEvents();
    }
    delete house;
  }

  void Game::stop() {
    renderer.shutdown();
  }
}
