#pragma once

#include <Material.hpp>
#include <Physics.hpp>
#include <Transform.hpp>

namespace ej {

class ResRenderable;
class Terrain {
  public:
    void load();
    void render();
  private:
    ResRenderable *renderableGround;
    Transform transformGround;
    Material materialGround;
    PhysicsBox physicsBox;

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
