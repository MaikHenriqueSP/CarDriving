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

void Camera::dolly() {
  auto counterForward = m_vehicle->getForward() * -1.0f;
  auto speed = m_vehicle->getSpeed();

  float direction = speed != 0 ? (speed / glm::abs(speed)) : 0;

  float xTarget = m_at.x + counterForward.x * 2.5f * direction;
  float zTarget = m_at.z + counterForward.z * 2.5f * direction;

  auto eyeTargetPosition = glm::vec3(xTarget, m_eye.y, zTarget);
  auto eyeForward = glm::normalize(eyeTargetPosition - m_eye) * direction;

  glm::vec3 atForward = glm::normalize(m_at - m_eye);

  m_eye += eyeForward * speed;
  m_at += atForward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {  
  glm::vec3 forward = glm::normalize(m_at - m_eye);
  glm::vec3 left = glm::cross(m_up, forward);

  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float deltaTime) {
  glm::mat4 transform{glm::mat4(1.0f)};

  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(
      transform, glm::radians(m_vehicle->getRotationFactor(deltaTime)), m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_vehicle->getPosition().x, m_at.y,
                               m_vehicle->getPosition().z, 1.0f);

  computeViewMatrix();
}
