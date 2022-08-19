#include <System.hpp>

#include <cassert>

namespace ej {

void SystemMsg::addInt(int i) {
  ints.push_back(i);
}

int SystemMsg::getInt(unsigned index) {
  assert(index < numInt() && "Invalid index");
  return ints[index];
}

std::string SystemMsg::getMsg() {
  return msg;
}

std::string SystemMsg::getSystem() {
  return system;
}

unsigned SystemMsg::numInt() {
  return ints.size();
}

std::vector<System*> System::systems = {};

}
