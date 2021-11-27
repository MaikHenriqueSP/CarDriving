#include "camera.hpp"
#include <fmt/core.h>
#include <glm/gtc/matrix_transform.hpp>

void Camera::initialize() {
  m_at = m_vehicle->getPosition();
  m_at.y = m_eye.y;
}

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 5.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly() { //move no eixo z  
  glm::vec3 forward = glm::normalize(m_at - m_eye);
  
  auto eye_target = m_vehicle->getForward() * -1.0f;
  auto speed = m_vehicle->getSpeed();

  m_eye = glm::vec3(m_at.x + eye_target.x * 3.5f, m_eye.y, m_at.z + eye_target.z * 3.5f);
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) { //dir e esq
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_at - m_eye);
  // Compute vector to the left
  glm::vec3 left = glm::cross(m_up, forward);

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}


void Camera::pan(float speed) { //girar no eixo y
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, glm::radians(m_vehicle->getRotationFactor(speed)), m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_vehicle->getPosition().x,m_at.y, m_vehicle->getPosition().z, 1.0f);  

  computeViewMatrix();
}

void Camera::setVehicle(Vehicle* vehicle) {
  m_vehicle = vehicle;
}
