#pragma once

#include <string>

namespace ej {

class ResScript;
namespace Script {
  void createScript(std::string src, ResScript &script);
  void load(ResScript& script);
  void update(ResScript& script, double delta);
};

}
