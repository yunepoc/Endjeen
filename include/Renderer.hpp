#pragma once

#include <Camera.hpp>
#include <Resource.hpp>
#include <System.hpp>
#include <vector>

namespace ej {

class Material;
class Transform;
class Renderer: public System {
  public:
    float getFPS();
    virtual void load();
    void render(ResRenderable& renderable, Material& material, Transform& transform, Camera &camera);
    void renderBefore();
    void setWireframeMode(bool enabled);
    virtual void shutdown();
    static void createRenderable(std::vector<float> &data, std::vector<unsigned> &indices, ResRenderable &renderable);
    static void createTexture(unsigned char* data, unsigned width, unsigned height, ResTexture &texture);
    static void compileShader(std::string source, ResShader &shader, bool vertex);
    static void linkShaders(ResShader &vertex, ResShader &fragment, ResShader &pipeline);
  private:
    virtual void receive(SystemMsg& msg);

    float lastFrame = 0.0;
    float FPS = 0.0;
};

}
