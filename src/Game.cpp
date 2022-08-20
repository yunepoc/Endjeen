#include <Game.hpp>

#include <Material.hpp>
#include <Terrain.hpp>
#include <Timer.hpp>
#include <Transform.hpp>

namespace ej {
  void Game::init(std::string root) {
    window = std::unique_ptr<Window>(new ej::Window(2560, 1440, "Endjeen"));

    resourceMgr.init(root);
    renderer.load();
    ui.load();
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

    // Model test
    ResRenderable &model = resourceMgr.get<ResRenderable>("backpack/backpack.obj");
    ResShader &modelShader = resourceMgr.get<ResShader>("default.shader");
    ResTexture &modelTexture = resourceMgr.get<ResTexture>("backpack.jpg");
    Material modelMaterial(modelShader, {&modelTexture});

    Transform transform;

    renderer.setWireframeMode(false);
    while (window->isOpen()) {
      // Compute delta
      float frame = Timer::getTime();
      delta = frame - lastFrame;
      lastFrame = frame;

      camera.update(delta);
      renderer.renderBefore();
      terrain.render(renderer, camera);
      if (showGrid)
        renderer.render(grid, gridMaterial, gridTransform, camera);
      //renderer.render(model, modelMaterial, transform, camera);
      ui.render();
      window->swapBuffers();
      window->pollEvents();
    }
  }

  void Game::stop() {
    renderer.shutdown();
  }
}
