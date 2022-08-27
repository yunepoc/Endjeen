#include <Renderer.hpp>

#include <App.hpp>
#include <Debug.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <Material.hpp>
#include <Resource.hpp>
#include <Timer.hpp>
#include <Transform.hpp>

namespace ej {

void Renderer::compileShader(std::string source, ResShader &shader, bool vertex) {
  unsigned int handle;
  if (vertex)
    handle = glCreateShader(GL_VERTEX_SHADER);
  else
    handle = glCreateShader(GL_FRAGMENT_SHADER);
  const char* csource = source.c_str();
  glShaderSource(handle, 1, &csource, NULL);
  glCompileShader(handle);
  int success;
  char infoLog[512];
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
  if (!success) {
      glGetShaderInfoLog(handle, 512, NULL, infoLog);
      WARN("Cannot compile vertex shader: " << infoLog);
      // TODO: we could fallback to a default shader here
  }
  shader.handle = handle;
}

void Renderer::linkShaders(ResShader &vertex, ResShader &fragment, ResShader &pipeline) {
  unsigned int handle = glCreateProgram();
  glAttachShader(handle, vertex.handle);
  glAttachShader(handle, fragment.handle);
  glLinkProgram(handle);
  int success;
  char infoLog[512];
  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(handle, 512, NULL, infoLog);
      WARN("Canot link shaders: " << infoLog);
      // TODO: we could fallback to a default shader here
  }
  glDeleteShader(vertex.handle);
  glDeleteShader(fragment.handle);
  pipeline.handle = handle;
}

void Renderer::createRenderable(std::vector<float> &data, std::vector<unsigned> &indices, ResRenderable &renderable) {
  renderable.vertexCount = data.size() / 8; // 3 position + 3 normal + 2 texcoords
  renderable.indiceCount = indices.size();
  glGenVertexArrays(1, &renderable.vao);
  glGenBuffers(1, &renderable.vbo);
  glGenBuffers(1, &renderable.ebo);
  glBindVertexArray(renderable.vao);

  glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::createLinesRenderable(std::vector<float> &data, ResRenderable &renderable) {
  renderable.vertexCount = data.size() / 3; // For lines renderable, we only store the position
  renderable.indiceCount = 0;
  glGenVertexArrays(1, &renderable.vao);
  glGenBuffers(1, &renderable.vbo);
  glBindVertexArray(renderable.vao);

  glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::createTexture(unsigned char* data, unsigned width, unsigned height, ResTexture &texture, bool hasAlpha) {
  glGenTextures(1, &texture.handle);
  glBindTexture(GL_TEXTURE_2D, texture.handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (hasAlpha)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Renderer::deleteRenderable(ResRenderable &renderable) {
  glDeleteVertexArrays(1, &renderable.vao);
  glDeleteBuffers(1, &renderable.vbo);
  if (renderable.indiceCount != 0)
    glDeleteBuffers(1, &renderable.ebo);
}

float Renderer::getFPS() {
  return FPS;
}

void Renderer::load() {
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    ERROR("Cannot initialize renderer");
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.605,0.664,0.745,1.0);
  glLineWidth(5.0);
  App::instance().getUI().addDebugBoolean("Wireframe mode", &wireframe);
}

void Renderer::receive(SystemMsg& msg) {
  if (msg.getSystem() != "window" || msg.getMsg() != "resize")
    return;
  assert(msg.numInt() == 2 && "Unexpected message");
  int width = msg.getInt(0);
  int height = msg.getInt(1);
  glViewport(0, 0, width, height);
}

void Renderer::render(ResRenderable &renderable, Material &material, Transform& transform) {

  Camera &camera = App::instance().getCamera();

  ResShader &shader = material.getShader();

  glUseProgram(shader.handle);

  // Set model matrix uniform
  glm::mat4 matrixModel = transform.getModelMatrix();
  GLint loc = glGetUniformLocation(shader.handle, "matrixModel");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrixModel));

  // Set view matrix uniform
  glm::mat4 matrixView = camera.getViewMatrix();
  loc = glGetUniformLocation(shader.handle, "matrixView");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrixView));

  // Set projection matrix uniform
  glm::mat4 matrixProjection = camera.getProjectionMatrix();
  loc = glGetUniformLocation(shader.handle, "matrixProjection");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrixProjection));

  // Set material uniforms
  loc = glGetUniformLocation(shader.handle, "textureCount");
  glUniform1ui(loc, material.getTextureCount());
  for (auto i=0; i<material.getTextureCount(); i++) {
    loc = glGetUniformLocation(shader.handle, ("texture" + std::to_string(i)).c_str());
    glUniform1i(loc, i);
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, material.getTexture(i).handle);
  }
  for (auto &p: material.getUniformsBool()) {
    loc = glGetUniformLocation(shader.handle, p.first.c_str());
    glUniform1i(loc, p.second);
  }

  // Set other uniforms
  loc = glGetUniformLocation(shader.handle, "elapsedTime");
  glUniform1f(loc, Timer::getTime());

  glBindVertexArray(renderable.vao);
  if (renderable.indiceCount == 0) {
    glDrawArrays(GL_LINES, 0, renderable.vertexCount);
  } else
    glDrawElements(GL_TRIANGLES, renderable.indiceCount, GL_UNSIGNED_INT, 0);
}

void Renderer::renderBefore() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // Compute fps
  float frame = Timer::getTime();
  float elapsed = frame - lastFrame;
  FPS = 1.0 / elapsed;
  lastFrame = frame;
}

void Renderer::shutdown() {
}

}
