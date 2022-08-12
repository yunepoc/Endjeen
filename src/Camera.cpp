#include <Camera.hpp>

#include <Game.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Input.hpp>

namespace ej {

Camera::Camera() {
  position = {0.0, 8.0, zOffset};
}

glm::mat4 Camera::getProjectionMatrix() {
  float fov = 45.0f;
  float near = 1.0f;
  float far = 100.0f;
  glm::uvec2 size = Game::instance().getWindow().getSize();
  float ratio = (float)size.x / (float)size.y;
  glm::mat4 matrixProjection = glm::perspective(glm::radians(fov), (float)(ratio), near, far);
  return matrixProjection;
}

glm::mat4 Camera::getViewMatrix() {
  // position, target, up
  return glm::lookAt(glm::vec3(position.x, position.y, position.z), glm::vec3(position.x, 0.0f, position.z-zOffset), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::update() {
  float speed = 0.0003;
  glm::vec2 translation(0.0f);
  if (Input::keyDown(Input::Key::A))
    translation.x -= 1.0;
  if (Input::keyDown(Input::Key::D))
    translation.x += 1.0;
  if (Input::keyDown(Input::Key::W))
    translation.y -= 1.0;
  if (Input::keyDown(Input::Key::S))
    translation.y += 1.0;
  if (translation.x != 0.0 || translation.y != 0.0)
    translation = glm::normalize(translation);
  translation *= speed;
  position.x += translation.x;
  position.z += translation.y;
}

}
