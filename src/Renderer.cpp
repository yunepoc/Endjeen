#include <Renderer.hpp>

#include <Debug.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Resource.hpp>

namespace ej {

void Renderer::createShader(unsigned int &handle) {
  const char *vertexSrc = "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 matrixView;\n"
      "uniform mat4 matrixProjection;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = matrixProjection * matrixView * vec4(aPos, 1.0f);\n"
      "}\0";
  const char *fragmentSrc = "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(0.5f, 0.6f, 0.5f, 1.0f);\n"
      "}\n\0";
  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSrc, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      WARN("Cannot compile vertex shader: " << infoLog);
  }
  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      WARN("Cannot compile fragment shader: " << infoLog);
  }
  // link shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  handle = shaderProgram;
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
