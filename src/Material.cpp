#include <Material.hpp>

#include <cassert>
#include <Game.hpp>

namespace ej {

Material::Material(): shader(&Game::instance().getResourceMgr().get<ResShader>("default.shader")) {
}

void Material::addTexture(ResTexture &texture) {
  textures.push_back(&texture);
}

ResShader& Material::getShader() {
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

}
