#include <Material.hpp>

#include <cassert>

namespace ej {

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

}
