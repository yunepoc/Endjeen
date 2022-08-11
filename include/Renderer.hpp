#pragma once

#include <Resource.hpp>
#include <System.hpp>

namespace ej {

class Renderer: public System {
  public:
    virtual void load();
    void render(ResRenderable& renderable, ResShader& shader);
    void renderBefore();
    virtual void shutdown();
    static void createRenderable(unsigned int &vao, unsigned int &vbo);
    static void createShader(unsigned int &handle);
};

}