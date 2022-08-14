#pragma once

#include <Camera.hpp>
#include <Resource.hpp>
#include <System.hpp>
#include <vector>

namespace ej {

class Renderer: public System {
  public:
    virtual void load();
    void render(ResRenderable& renderable, ResShader& shader, Camera &camera);
    void renderBefore();
    void setWireframeMode(bool enabled);
    virtual void shutdown();
    static void createRenderable(std::vector<float> vertices, ResRenderable &renderable);
    static void compileShader(std::string source, ResShader &shader, bool vertex);
    static void linkShaders(ResShader &vertex, ResShader &fragment, ResShader &pipeline);
};

}
