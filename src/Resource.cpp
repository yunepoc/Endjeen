#include <Resource.hpp>

#include <Renderer.hpp>

namespace ej {

void ResRenderable::load() {
  Renderer::createRenderable(vao, vbo);
}

void ResShader::load() {
  Renderer::createShader(handle);
}

void ResourceManager::init(std::string root) {
  this->root = root;
}

}
