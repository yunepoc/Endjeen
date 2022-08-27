#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace ej {

class ResShader;
class ResTexture;
class Material {
  public:
    Material() {}
    Material(ResShader &shader): shader(&shader) {}
    Material(ResShader &shader, std::vector<ResTexture*> textures): shader(&shader), textures(textures) {}
    void addTexture(ResTexture &texture);
    ResShader& getShader();
    ResTexture& getTexture(unsigned index);
    unsigned getTextureCount();
    std::unordered_map<std::string, bool> getUniformsBool() { return uniformsBool; }
    void setShader(ResShader &shader);
    void setUniformBool(std::string name, bool value);
  private:
    ResShader *shader = nullptr;
    std::vector<ResTexture*> textures;
    std::unordered_map<std::string, bool> uniformsBool;
};

}
