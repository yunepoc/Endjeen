#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace ej {

struct Resource {
  virtual void load() = 0;
};

struct ResRenderable: public Resource {
  void load();
  unsigned int vao;
  unsigned int vbo;
};

struct ResShader: public Resource {
  void load();
  unsigned int handle;
};

class ResourceManager {
  public:
    void init(std::string root);
    template<typename Res>
    Res& get(std::string key) {
      if (root == "")
        throw 0;
      std::unique_ptr<Resource> &inCache = cache[key];
      if (inCache) {
        Resource* res = inCache.get();
        Res* result = dynamic_cast<Res*>(res);
        if (result)
          return *result;
        throw 0;
      }
      Res* res = new Res();
      inCache.reset(res);
      res->load();
      return *res;
    }
  private:
    std::string root = "";
    std::unordered_map<std::string, std::unique_ptr<Resource>> cache;
};

}
