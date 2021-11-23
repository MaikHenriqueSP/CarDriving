#include "vehicle.hpp"
#include <fmt/core.h>

void Vehicle::update(float deltaTime) {
    if (m_actionData.m_input[static_cast<size_t>(Action::Forward)]) {
        m_position += m_forward * m_speed * deltaTime;
    } else if (m_actionData.m_input[static_cast<size_t>(Action::Backward)]) {
        m_position += m_forward * m_speed * deltaTime * (-1.0f);

    }
    m_modelMatrix = glm::translate(glm::mat4{1.0f}, m_position);
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));    
}

void Vehicle::setAction(Action action) {
    m_actionData.m_input.set(static_cast<size_t>(action));
}

void Vehicle::resetAction(Action action) {
    m_actionData.m_input.reset(static_cast<size_t>(action));
}