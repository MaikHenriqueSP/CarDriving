#include "vehicle.hpp"

void Vehicle::accelerate(float deltaTime, int direction) {
    m_position += m_forward * m_speed;
    m_modelMatrix = glm::translate(glm::mat4{1.0f}, m_position);
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
}