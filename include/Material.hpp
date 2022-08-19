#pragma once

#include <vector>

namespace ej {

class ResShader;
class ResTexture;
class Material {
  public:
    Material();
    Material(ResShader &shader): shader(&shader) {}
    Material(ResShader &shader, std::vector<ResTexture*> textures): shader(&shader), textures(textures) {}
    void addTexture(ResTexture &texture);
    ResShader& getShader();
    ResTexture& getTexture(unsigned index);
    unsigned getTextureCount();
    void setShader(ResShader &shader);
  private:
    ResShader *shader = nullptr;
    std::vector<ResTexture*> textures;
};

}
