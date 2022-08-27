#include <Material.hpp>

#include <cassert>

namespace ej {

void Material::addTexture(ResTexture &texture) {
  textures.push_back(&texture);
}

ResShader& Material::getShader() {
  assert(shader != nullptr);
  return *shader;
}

ResTexture& Material::getTexture(unsigned index) {
  assert(index < textures.size() && "Invalid texture index");
  return *textures[index];
}

unsigned Material::getTextureCount() {
  return textures.size();
}

void Material::setShader(ResShader &shader) {
  this->shader = &shader;
}

void Material::setUniformBool(std::string name, bool value) {
  uniformsBool[name] = value;
}

}
