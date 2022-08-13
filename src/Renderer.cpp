#include <Renderer.hpp>

#include <Debug.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Resource.hpp>

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

void Renderer::createRenderable(unsigned int &vao, unsigned int &vbo) {
  const float vertices[] = {
    -0.5f, 0.0, -0.5f, // top left
     0.5f, 0.0, -0.5f, // top right
     0.5f, 0.0,  0.5f, // bot right

    -0.5f, 0.0, -0.5f, // top left
     0.5f, 0.0,  0.5f, // bot right
    -0.5f, 0.0,  0.5f, // bot left
  };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void Renderer::load() {
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    ERROR("Cannot initialize renderer");
  glClearColor(0.605,0.664,0.745,1.0);
}

void Renderer::render(ResRenderable &renderable, ResShader &shader, Camera &camera) {

  glUseProgram(shader.handle);

  // Set view matrix uniform
  glm::mat4 matrixView = camera.getViewMatrix();
  GLint loc = glGetUniformLocation(shader.handle, "matrixView");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrixView));

  // Set projection matrix uniform
  glm::mat4 matrixProjection = camera.getProjectionMatrix();
  loc = glGetUniformLocation(shader.handle, "matrixProjection");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrixProjection));

  glBindVertexArray(renderable.vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::renderBefore() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::shutdown() {
}

}
