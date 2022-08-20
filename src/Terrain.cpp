#include <Terrain.hpp>

#include <Game.hpp>
#include <Resource.hpp>
#include <stb_image/stb_image.h>

namespace ej {

Terrain::Terrain() {
  renderableGround = new ResRenderable();
  ResourceManager& resourceMgr = Game::instance().getResourceMgr();

  // Gnerate mesh data & indices from heightmap
  loadHeightmap();
  // Compute terrain normals
  computeNormals();
  Renderer::createRenderable(data, indices, *renderableGround);

  // Ground
  ResShader &shader = resourceMgr.get<ResShader>("terrain.shader");
  transformGround.setScale({0.2f, 1.0f, 0.2f});
  ResTexture &textureGround = resourceMgr.get<ResTexture>("terrain.png");
  ResTexture &textureCliff = resourceMgr.get<ResTexture>("cliff.jpg");
  materialGround.setShader(shader);
  materialGround.addTexture(textureGround);
  materialGround.addTexture(textureCliff);

  // Water
  ResShader &shaderWater = resourceMgr.get<ResShader>("water.shader");
  std::string planeKey = "plane5x5x1.std";
  renderableWater = &resourceMgr.get<ResRenderable>(planeKey);
  ResTexture &textureWater = resourceMgr.get<ResTexture>("water.jpg");
  materialWater.setShader(shaderWater);
  materialWater.addTexture(textureWater);
  auto pos = transformGround.getPosition();
  transformWater.setPosition({pos.x, pos.y-0.2, pos.z});
  transformWater.setScale({4.0, 1.0, 4.0});
}

void Terrain::render(Renderer& renderer, Camera& camera) {
  renderer.render(*renderableGround, materialGround, transformGround, camera);
  renderer.render(*renderableWater, materialWater, transformWater, camera);
}

void Terrain::loadHeightmap() {
  int nrChannels;
  std::string path = Game::instance().getResourceMgr().getFilePath("/heightmap.png");
  unsigned char *heights = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (!heights)
    ERROR("Cannot load heightmap");
  // Generate plane triangles
  for (int y=0; y<height; y++) {
    for (int x=0; x<width; x++) {
      // position
      data.push_back(x);
      int ty = height - 1 - y;
      int index = (ty*width+x) * 3;
      float height = heights[index] / 255.0;
      data.push_back(-height);
      data.push_back(-y);
      // normal
      data.push_back(0.0f);
      data.push_back(1.0f);
      data.push_back(0.0f);
      // texcoords
      data.push_back(x/20.0);
      data.push_back(y/20.0);
    }
  }
  for (int y=0; y<height-1; y++)
    for (int x=0; x<width-1; x++) {
      int first = x + y*(width);
      indices.push_back(first+width);
      indices.push_back(first+width+1);
      indices.push_back(first+1);
      indices.push_back(first+width);
      indices.push_back(first+1);
      indices.push_back(first);
    }
  stbi_image_free(heights);
}

void Terrain::computeNormals() {
  for (int y=1; y<height-1; y++)
    for (int x=1; x<width-1; x++) {
      glm::vec3 pLeft(x-1, getVerticeHeight(x-1, y), -y);
      glm::vec3 pTop(x, getVerticeHeight(x, y+1), -(y+1));
      glm::vec3 pRight(x+1, getVerticeHeight(x+1, y), -y);
      glm::vec3 pDown(x, getVerticeHeight(x, y-1), -(y-1));

      glm::vec3 pCur(x, getVerticeHeight(x, y), -y);

      // Compute normals of the surrounding triangles
      glm::vec3 normalTL(glm::cross(pLeft-pCur, pTop-pCur)); if (normalTL.y < 0) normalTL *= -1;
      glm::vec3 normalTR(glm::cross(pTop-pCur, pRight-pCur)); if (normalTR.y < 0) normalTR *= -1;
      glm::vec3 normalBR(glm::cross(pRight-pCur, pDown-pCur)); if (normalBR.y < 0) normalBR *= -1;
      glm::vec3 normalBL(glm::cross(pDown-pCur, pLeft-pCur)); if (normalBL.y < 0) normalBL *= -1;
      // Compute vertex normal
      glm::vec3 normal = glm::normalize(normalTL + normalTR + normalBR + normalBL);

      unsigned index = y * width + x;
      data[index * 8 + 3] = normal.x;
      data[index * 8 + 4] = normal.y;
      data[index * 8 + 5] = normal.z;
    }
}

float Terrain::getVerticeHeight(unsigned x, unsigned y) {
  assert(x < width && y < height && "Invalid vertex coordinate to get height");
  unsigned index = y * width + x;
  return data[index * 8 + 1];
}

}
