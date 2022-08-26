#include <System.hpp>

#include <cassert>

namespace ej {

void SystemMsg::addInt(int i) {
  ints.push_back(i);
}

void SystemMsg::addString(std::string s) {
  strings.push_back(s);
}

int SystemMsg::getInt(unsigned index) {
  assert(index < numInt() && "Invalid index");
  return ints[index];
}

std::string SystemMsg::getString(unsigned index) {
  assert(index < numString() && "Invalid index");
  return strings[index];
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

unsigned SystemMsg::numString() {
  return strings.size();
}

std::vector<System*> System::systems = {};

}
