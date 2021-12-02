#include "vehicle.hpp"

void Vehicle::update(float deltaTime) {
  float rotateFactor = getRotationFactor(deltaTime);
  updateSpeed(deltaTime);

  updateDirection(rotateFactor);
  m_position += m_forward * m_speed;
  updateOffLimits();
  m_modelMatrix = glm::translate(glm::mat4{1.0f}, m_position);
  m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotationAngle),
                              glm::vec3(0, 1, 0));
}

void Vehicle::updateAction(Uint32 eventType, Action action) {
  if (eventType == SDL_KEYDOWN) {
    m_actionData.m_input.set(static_cast<size_t>(action));
  }

  if (eventType == SDL_KEYUP) {
    m_actionData.m_input.reset(static_cast<size_t>(action));
  }
}

void Vehicle::updateDirection(float rotateFactor) {
  if (rotateFactor == 0) return;
  m_rotationAngle += rotateFactor;

  float x = m_forward.x;
  float z = m_forward.z;
  float radiansRotateFactor = glm::radians(rotateFactor);

  m_forward.z = z * glm::cos(radiansRotateFactor) - x * glm::sin(radiansRotateFactor);
  m_forward.x = z * glm::sin(radiansRotateFactor) + x * glm::cos(radiansRotateFactor);
}

float Vehicle::getSpeed() { return m_speed; }

glm::vec3 Vehicle::getPosition() { return m_position; }

float Vehicle::getRotationFactor(float deltaTime) {
  float rotateFactor = 0.0f;

  if (!m_actionData.m_input[static_cast<size_t>(Action::Forward)] &&
      !m_actionData.m_input[static_cast<size_t>(Action::Backward)]) {
    return 0.0f;
  }

  if (m_actionData.m_input[static_cast<size_t>(Action::Left)]) {
    rotateFactor += m_baseSteeringAngle * deltaTime;
  }

  if (m_actionData.m_input[static_cast<size_t>(Action::Right)]) {
    rotateFactor -= m_baseSteeringAngle * deltaTime;
  }

  if (m_actionData.m_input[static_cast<size_t>(Action::Backward)]) {
    rotateFactor *= -1.0f;
  }

  return rotateFactor;
}

void Vehicle::updateSpeed(float deltaTime) {
  m_speed = 0.0f;

  if (m_actionData.m_input[static_cast<size_t>(Action::Forward)] &&
      m_actionData.m_input[static_cast<size_t>(Action::Backward)]) {
    return;
  }

  if (m_actionData.m_input[static_cast<size_t>(Action::Forward)]) {
    m_speed = m_maxSpeed * deltaTime;
  }

  if (m_actionData.m_input[static_cast<size_t>(Action::Backward)]) {
    m_speed = - m_maxSpeed * deltaTime;
  }
}

glm::vec3 Vehicle::getForward() { return m_forward; }

void Vehicle::setBorderLimit(BorderLimits border, float location) {
  m_borderLimitsMap[border] = location;
}

void Vehicle::updateOffLimits() {
  if (m_position.x < m_borderLimitsMap[BorderLimits::Left] + m_scaleFactor) {
    m_position.x = m_borderLimitsMap[BorderLimits::Left] + m_scaleFactor;
  }

  if (m_position.x > m_borderLimitsMap[BorderLimits::Right] - m_scaleFactor) {
    m_position.x = m_borderLimitsMap[BorderLimits::Right] - m_scaleFactor;
  }

  if (m_position.z < m_borderLimitsMap[BorderLimits::Backward] + m_scaleFactor) {
    m_position.z = m_borderLimitsMap[BorderLimits::Backward] + m_scaleFactor;
  }

  if (m_position.z > m_borderLimitsMap[BorderLimits::Front] - m_scaleFactor) {
    m_position.z = m_borderLimitsMap[BorderLimits::Front] - m_scaleFactor;
  }

}