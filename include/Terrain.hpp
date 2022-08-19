#pragma once

#include <Material.hpp>
#include <Transform.hpp>

namespace ej {

class Camera;
class Renderer;
class ResRenderable;
class ResShader;
class Terrain {
  public:
    Terrain();
    void render(Renderer& renderer, Camera& camera);
  private:
    ResRenderable *renderableGround;
    Transform transformGround;
    Material materialGround;

    ResRenderable *renderableWater;
    Transform transformWater;
    Material materialWater;

    void loadHeightmap();
    void computeNormals();

    int width, height; // # vertices

    float getVerticeHeight(unsigned x, unsigned y);
    std::vector<float> data; // Terrain mesh data
    std::vector<unsigned> indices; // Terrain mesh indices
};

}
