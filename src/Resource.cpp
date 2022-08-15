#include <Resource.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <Debug.hpp>
#include <filesystem>
#include <fstream>
#include <Game.hpp>
#include <regex>
#include <Renderer.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace ej {

static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<float> &data, std::vector<unsigned> &indices) {
  unsigned offset = data.size() / 8;
  for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
    // position
    data.push_back(mesh->mVertices[i].x);
    data.push_back(mesh->mVertices[i].y);
    data.push_back(mesh->mVertices[i].z);
    // normal
    if (mesh->mNormals != nullptr) {
      data.push_back(mesh->mNormals[i].x);
      data.push_back(mesh->mNormals[i].y);
      data.push_back(mesh->mNormals[i].z);
    } else {
      data.push_back(0.0f);
      data.push_back(1.0f);
      data.push_back(0.0f);
    }
    // texcoords
    if(mesh->mTextureCoords[0]) {
      data.push_back(mesh->mTextureCoords[0][i].x);
      data.push_back(mesh->mTextureCoords[0][i].y);
    } else {
      data.push_back(0.0);
      data.push_back(0.0);
    }
  }
  // indices
  for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for(unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j] + offset);
  }
}

static void processMeshNode(aiNode* node, const aiScene* scene, std::vector<float> &data, std::vector<unsigned> &indices) {
 for(unsigned int i = 0; i < node->mNumMeshes; i++) {
   aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
   processMesh(mesh, scene, data, indices);
 }
 for(unsigned int i = 0; i < node->mNumChildren; i++)
   processMeshNode(node->mChildren[i], scene, data, indices);
}

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
      std::vector<float> data;
      for (int y=0; y<=height; y++)
        for (int x=0; x<=width; x++) {
          // position
          data.push_back(x);
          data.push_back(0.0f);
          data.push_back(-y);
          // normal
          data.push_back(0.0f);
          data.push_back(1.0f);
          data.push_back(0.0f);
          // texcoords
          data.push_back(x/*x/(float)width*/);
          data.push_back(y/*y/(float)height*/);
        }
      std::vector<unsigned> indices;
      for (int y=0; y<height; y++)
        for (int x=0; x<width; x++) {
          int first = x + y*(width+1);
          indices.push_back(first+width+1);
          indices.push_back(first+width+2);
          indices.push_back(first+1);
          indices.push_back(first+width+1);
          indices.push_back(first+1);
          indices.push_back(first);
        }
      Renderer::createRenderable(data, indices, *this);
      return;
    }
    // Try to match plane: planeWxH.std. This is WxH plane built with two triangles.
    regex = (R"(plane(\d+)x(\d+).std)");
    if(std::regex_search(key, match, regex) && match.size() == 3) {
      float width = std::stoi(match[1]);
      float height = std::stoi(match[2]);
      // Generate plane triangles
      std::vector<float> data = {
         0.0f, 0.0f, -height, // top left position
         0.0f, 1.0f,    0.0f, //          normal
         0.0f, 1.0f,          //          texcoords
        width, 0.0f, -height, // top right position
         0.0f, 1.0f,    0.0f, //          normal
         1.0f, 1.0f,          //          texcoords
        width, 0.0f,    0.0f, // bot right position
         0.0f, 1.0f,    0.0f, //          normal
         1.0f, 0.0f,          //          texcoords
         0.0f, 0.0f,    0.0f, // bot left position
         0.0f, 1.0f,    0.0f, //          normal
         0.0f, 0.0f,          //          texcoords
      };
      std::vector<unsigned> indices = {
        0, 1, 2, 0, 2, 3
      };
      Renderer::createRenderable(data, indices, *this);
      return;
    }
    ERROR("Invalid std renderable key: \"" << key << "\"");
  }
  std::string path = root + "/models/" + key;
  if (!std::filesystem::exists(path)) {
    WARN("Cannot load renderable \"" << key << "\"");
    return;
  }
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    // TODO: we could fallback to a default renderable here
    WARN("Cannot load renderable \"" << key << "\"");
    return;
  }
  std::vector<float> data;
  std::vector<unsigned> indices;
  processMeshNode(scene->mRootNode, scene, data, indices);
  Renderer::createRenderable(data, indices, *this);
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

void ResTexture::load(std::string root, std::string key) {
  std::string path = root + "/textures/" + key;
  int width, height, nrChannels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (!data) {
    WARN("Invalid texture \"" << key << "\"");
    // TODO: we could fallback to a default texture here
    handle = 0;
    return;
  }
  Renderer::createTexture(data, width, height, *this);
  stbi_image_free(data);
}

void ResourceManager::init(std::string root) {
  if (!std::filesystem::exists(root))
    ERROR("Invalid root path for game content: \"" << root << "\"");
  this->root = root;
}

}
