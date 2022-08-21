#include <Debug.hpp>
#include <App.hpp>

int main() {

  LOG("Sarting game...");
  ej::App& app = ej::App::instance();
  app.init("../../../GameContent/");
  app.run();
  app.stop();

  return 0;
}
