#include <Camera.hpp>

#include <App.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Input.hpp>

namespace ej {

Camera::Camera() {
  transform.setPosition({0, 10, 0});
  transform.setRotation({-55, 0, 0});
}

glm::mat4 Camera::getProjectionMatrix() {
  float fov = 45.0f;
  float near = 1.0f;
  float far = 500.0f;
  glm::uvec2 size = App::instance().getWindow().getSize();
  float ratio = (float)size.x / (float)size.y;
  glm::mat4 matrixProjection = glm::perspective(glm::radians(fov), (float)(ratio), near, far);
  return matrixProjection;
}

glm::mat4 Camera::getViewMatrix() {
  return glm::inverse(transform.getModelMatrix());
}

void Camera::receive(SystemMsg& msg) {
  if (msg.getSystem() != "input" && msg.getMsg() != "scroll")
    return;
  // Zoom in/out
  int x = msg.getInt(0);
  int y = msg.getInt(1);
  glm::vec3 forward = transform.forward();
  forward.x *= y;
  forward.y *= y;
  forward.z *= y;
  transform.translate(forward);
}

std::pair<glm::vec3, glm::vec3> Camera::screenToRay(glm::uvec2 coordinates) {
  unsigned x = coordinates.x;
  unsigned y = coordinates.y;
  auto size = App::instance().getWindow().getSize();

  auto clickx = ((double)(x)/(double)(size.x) - 0.5f) * 2.0f;
  auto clicky = ((double)(size.y-y)/(double)(size.y) - 0.5f) * 2.0f;

  glm::vec4 rayStart(clickx,clicky,-1.0f,1.0f);
  glm::vec4 rayEnd(clickx,clicky, 0.0f,1.0f);

  glm::mat4 view = getViewMatrix();
  glm::mat4 projection = getProjectionMatrix();

  glm::mat4 M = glm::inverse(projection * view);
  glm::vec4 sta = M * rayStart; sta /= sta.w;
  glm::vec4 end = M * rayEnd;   end /= end.w;

  glm::vec3 dir(end - sta);
  dir = glm::normalize(dir);

  return {sta, dir};
}

void Camera::update(float delta) {
  // Translation
  float speed = 8 * delta;
  glm::vec2 translation(0.0f);
  if (Input::keyDown(Input::Key::A) || Input::keyDown(Input::Key::Left))
    translation.x -= 1.0;
  if (Input::keyDown(Input::Key::D) || Input::keyDown(Input::Key::Right))
    translation.x += 1.0;
  if (Input::keyDown(Input::Key::W) || Input::keyDown(Input::Key::Up))
    translation.y -= 1.0;
  if (Input::keyDown(Input::Key::S) || Input::keyDown(Input::Key::Down))
    translation.y += 1.0;
  if (translation.x != 0.0 || translation.y != 0.0)
    translation = glm::normalize(translation);
  translation *= speed;
  auto position = transform.getPosition();
  position.x += translation.x;
  position.z += translation.y;
  transform.setPosition(position);
  // Zoom
  speed = 10 * delta;
  if (Input::keyDown(Input::Key::Add) || Input::keyDown(Input::Key::Subtract)) {
    int mod = Input::keyDown(Input::Key::Subtract) ? -1 : 1;
    glm::vec3 forward = transform.forward();
    forward.x *= speed * mod;
    forward.y *= speed * mod;
    forward.z *= speed * mod;
    transform.translate(forward);
  }
}

}
