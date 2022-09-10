#pragma once

#include <string>

namespace ej {

class GameObject;
class ResScript;
namespace Script {
  void createScript(std::string src, ResScript &script);
  void load(ResScript& script, GameObject& object);
  void update(ResScript& script, double delta);
};

}
