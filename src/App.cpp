#include <App.hpp>

#include <Material.hpp>
#include <Timer.hpp>
#include <Transform.hpp>

namespace ej {
  void App::init(std::string root) {
    window = std::unique_ptr<Window>(new ej::Window(2560, 1440));

    resourceMgr.init(root);
    // The order of the loads is important here
    renderer.load();
    physics.load();
    window->load();
    input.load();
    ui.load();
    gameContent.load();
    game.load();
    ui.addDebugBoolean("Show grid", &showGrid);
  }

  void App::run() {

    // Grid
    ResRenderable &grid = resourceMgr.get<ResRenderable>("plane1000x1000.std");
    ResShader &gridShader = resourceMgr.get<ResShader>("grid.shader");
    Transform gridTransform;
    gridTransform.setPosition({-500.0f, 0.01f, 500.0f});
    Material gridMaterial(gridShader);

    while (window->isOpen()) {
      // Compute delta
      float frame = Timer::getTime();
      delta = frame - lastFrame;
      lastFrame = frame;
      // Update
      physics.update(delta);
      camera.update(delta);
      window->update();
      game.update();
      // Render
      renderer.renderBefore();
      if (showGrid)
        renderer.render(grid, gridMaterial, gridTransform);
      game.render();
      physics.render();
      ui.render();
      //
      window->swapBuffers();
      window->pollEvents();
    }
  }

  void App::stop() {
    renderer.shutdown();
  }
}
