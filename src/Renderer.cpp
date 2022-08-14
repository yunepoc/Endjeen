#include <Renderer.hpp>

#include <Debug.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Resource.hpp>
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

void Renderer::createRenderable(std::vector<float> vertices, ResRenderable &renderable) {
  renderable.vertexCount = vertices.size() / 3;
  glGenVertexArrays(1, &renderable.vao);
  glGenBuffers(1, &renderable.vbo);
  glBindVertexArray(renderable.vao);

  glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::load() {
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    ERROR("Cannot initialize renderer");
  glClearColor(0.605,0.664,0.745,1.0);
  glLineWidth(5.0);
}

void Renderer::render(ResRenderable &renderable, ResShader &shader, Transform& transform, Camera &camera) {

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

  glBindVertexArray(renderable.vao);
  glDrawArrays(GL_TRIANGLES, 0, renderable.vertexCount);
}

void Renderer::renderBefore() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::setWireframeMode(bool enabled) {
  if (enabled)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::shutdown() {
}

}
