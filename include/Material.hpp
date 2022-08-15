#pragma once

#include <vector>

namespace ej {

class ResShader;
class ResTexture;
class Material {
  public:
    Material(ResShader &shader): shader(&shader) {}
    Material(ResShader &shader, std::vector<ResTexture*> textures): shader(&shader), textures(textures) {}
    ResShader& getShader();
    ResTexture& getTexture(unsigned index);
    unsigned getTextureCount();
  private:
    ResShader *shader = nullptr;
    std::vector<ResTexture*> textures;
};

}
