#include "camera.hpp"

#include <fmt/core.h>

#include <glm/gtc/matrix_transform.hpp>

void Camera::initialize(Vehicle* vehicle) {
  m_vehicle = vehicle;
  m_at = m_vehicle->getPosition();
  m_at.y = m_eye.y;
}

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::update() {
  m_at = glm::vec4(m_vehicle->getPosition().x, m_at.y, m_vehicle->getPosition().z, 1.0f);
  
  auto counterForward = m_vehicle->getForward() * -1.0f;
  auto speed = m_vehicle->getSpeed();

  float direction = speed != 0 ? (speed / glm::abs(speed)) : 0;

  float xTarget = m_at.x + counterForward.x * m_zoom * direction;
  float zTarget = m_at.z + counterForward.z * m_zoom * direction;

  auto eyeTargetPosition = glm::vec3(xTarget, m_eye.y, zTarget);
  auto eyeForward = glm::normalize(eyeTargetPosition - m_eye) * direction;

  m_eye += eyeForward * speed;
  computeViewMatrix();
}
