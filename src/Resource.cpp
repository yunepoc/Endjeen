#include <Resource.hpp>

#include <Debug.hpp>
#include <filesystem>
#include <fstream>
#include <Game.hpp>
#include <regex>
#include <Renderer.hpp>

namespace ej {

void ResRenderable::load(std::string root, std::string key) {
  // The key represents an internal std object
  if (key.ends_with(".std")) {
    // Try to match plane: planeWxHx1.std. This is a WxH plane built with triangles
    // of size 1.
    std::regex regex(R"(plane(\d+)x(\d+)x1.std)");
    std::smatch match;
    if(std::regex_search(key, match, regex) && match.size() == 3) {
      int width = std::stoi(match[1]);
      int height = std::stoi(match[2]);
      // Generate plane triangles
      std::vector<float> vertices;
      std::vector<glm::vec3> square = {
        {0.0, 0.0, -1.0}, // top left
        {1.0, 0.0, -1.0}, // top right
        {1.0, 0.0,  0.0}, // bot right

        {0.0, 0.0, -1.0}, // top left
        {1.0, 0.0,  0.0}, // bot right
        {0.0, 0.0,  0.0}, // bot left
      };
      for (int x=0; x<width; x++)
        for (int y=0; y<height; y++) {
          for (auto p: square) {
            p.x += x;
            p.z -= y;
            vertices.push_back(p.x);
            vertices.push_back(p.y);
            vertices.push_back(p.z);
          }
        }
      Renderer::createRenderable(vertices, *this);
      return;
    }
    // Try to match plane: planeWxH.std. This is WxH plane built with two triangles.
    regex = (R"(plane(\d+)x(\d+).std)");
    if(std::regex_search(key, match, regex) && match.size() == 3) {
      int width = std::stoi(match[1]);
      int height = std::stoi(match[2]);
      // Generate plane triangles
      std::vector<float> vertices;
      vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(-height);
      vertices.push_back(width); vertices.push_back(0.0f); vertices.push_back(-height);
      vertices.push_back(width); vertices.push_back(0.0f); vertices.push_back(0.0f);

      vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(-height);
      vertices.push_back(width); vertices.push_back(0.0f); vertices.push_back(0.0f);
      vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);

      Renderer::createRenderable(vertices, *this);
      return;
    }
    ERROR("Invalid std renderable key: \"" << key << "\"");
  }
  std::vector<float> vertices = {
    -0.5f, 0.0, -0.5f, // top left
     0.5f, 0.0, -0.5f, // top right
     0.5f, 0.0,  0.5f, // bot right

    -0.5f, 0.0, -0.5f, // top left
     0.5f, 0.0,  0.5f, // bot right
    -0.5f, 0.0,  0.5f, // bot left
  };
  Renderer::createRenderable(vertices, *this);
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
