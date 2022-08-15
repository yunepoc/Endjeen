#include <Game.hpp>

#include <Material.hpp>
#include <Transform.hpp>

namespace ej {
  void Game::init(std::string root) {
    window = std::unique_ptr<Window>(new ej::Window(2560, 1440, "Endjeen"));

    resourceMgr.init(root);
    renderer.load();
  }

  void Game::run() {
    // Terrain
    ResRenderable &plane = resourceMgr.get<ResRenderable>("plane10x10x1.std");
    ResShader &shader = resourceMgr.get<ResShader>("terrain.shader");
    Transform transform;
    transform.setPosition({0.0f, 0.0f, 0.0f});
    ResTexture &texture = resourceMgr.get<ResTexture>("terrain.png");
    Material material(shader, {&texture});

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

    renderer.setWireframeMode(false);
    while (window->isOpen()) {
      camera.update();
      renderer.renderBefore();
      renderer.render(plane, material, transform, camera);
      renderer.render(grid, gridMaterial, gridTransform, camera);
      renderer.render(model, modelMaterial, transform, camera);
      window->swapBuffers();
      window->pollEvents();
    }
  }

  void Game::stop() {
    renderer.shutdown();
  }
}
