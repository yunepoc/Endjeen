#pragma once

#include <Debug.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace ej {

struct Resource {
  virtual void load(std::string root, std::string key) = 0;
};

struct ResRenderable: public Resource {
  void load(std::string root, std::string key);
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  unsigned int vertexCount;
  unsigned int indiceCount;
};

struct ResScript: public Resource {
  void load(std::string root, std::string key);
  void* handle;
  std::string source;
};

struct ResShader: public Resource {
  void load(std::string root, std::string key);
  unsigned int handle;
};

struct ResTexture: public Resource {
  void load(std::string root, std::string key);
  unsigned handle;
};

class ResourceManager {
  public:
    void init(std::string root);
    template<typename Res>
    Res& get(std::string key) {
      if (root == "")
        ERROR("ResourceManager has not been initialized");
      std::unique_ptr<Resource> &inCache = cache[key];
      if (inCache) {
        Resource* res = inCache.get();
        Res* result = dynamic_cast<Res*>(res);
        if (result)
          return *result;
        ERROR("Unexpected resource type in cache for resource \"" << key << "\"");
      }
      LOG("Loading resource \"" << key << "\"");
      Res* res = new Res();
      inCache.reset(res);
      res->load(root, key);
      return *res;
    }
    std::string getFilePath(std::string relative);
  private:
    std::string root = "";
    std::unordered_map<std::string, std::unique_ptr<Resource>> cache;
};

}
