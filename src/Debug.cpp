#include <Debug.hpp>

namespace ej {

std::string Debug::getLogPrefix() {
  std::string blue = "\033[1;34m";
  std::string none = "\033[0m";
  return blue + "[LOG]: " + none;
}

std::string Debug::getWarnPrefix() {
  std::string yellow = "\033[1;33m";
  std::string none = "\033[0m";
  return yellow + "[WARN]: " + none;
}

std::string Debug::getErrorPrefix() {
  std::string red = "\033[1;31m";
  std::string none = "\033[0m";
  return red + "[ERROR]: " + none;
}

}
