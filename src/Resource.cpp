#include <Resource.hpp>

#include <Debug.hpp>
#include <filesystem>
#include <fstream>
#include <Game.hpp>
#include <Renderer.hpp>

namespace ej {

void ResRenderable::load(std::string root, std::string key) {
  Renderer::createRenderable(vao, vbo);
}

void ResShader::load(std::string root, std::string key) {
  std::string path = root + "/shaders/" + key;
  if (!std::filesystem::exists(path)) {
    WARN("Cannot find shader \"" << path << "\"");
    // TODO: we could fallback to a default shader here
    handle = 0;
    return;
  }
  // .shader file containing a vertex and fragment shader key
  if (key.ends_with(".shader")) {
    std::ifstream f(path);
    bool hasVertex = false, hasFragment = false;
    std::string vertexKey, fragmentKey;
    for (std::string line; getline(f, line);) {
      if (hasVertex) {
        fragmentKey = line;
        hasFragment = true;
        break;
      } else {
        vertexKey = line;
        hasVertex = true;
      }
    }
    if (!hasVertex || !hasFragment) {
      WARN("Invalid shader format for \"" << key << "\"");
      // TODO: we could fallback to a default shader here
      handle = 0;
      return;
    }

    ResShader &vertex   = Game::instance().getResourceMgr().get<ResShader>(vertexKey);
    ResShader &fragment = Game::instance().getResourceMgr().get<ResShader>(fragmentKey);
    Renderer::linkShaders(vertex, fragment, *this);
  // .vertex or .fragment file containing a shader source
  } else if (key.ends_with(".vertex") || key.ends_with(".fragment")) {
    std::ifstream f(path);
    std::string source((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
    if (key.ends_with(".vertex"))
      Renderer::compileShader(source, *this, true);
    else
      Renderer::compileShader(source, *this, false);
  } else {
    WARN("Invalid shader format for \"" << key << "\"");
    // TODO: we could fallback to a default shader here
    handle = 0;
    return;
  }
}

void ResourceManager::init(std::string root) {
  if (!std::filesystem::exists(root))
    ERROR("Invalid root path for game content: \"" << root << "\"");
  this->root = root;
}

}
