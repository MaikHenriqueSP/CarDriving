#include "vehicle.hpp"
#include <fmt/core.h>

void Vehicle::update(float deltaTime) {
    float rotateFactor = 0.0f;
    // fmt::print("x={} | z={}\n", m_forward.x, m_forward.z);
    if (m_actionData.m_input[static_cast<size_t>(Action::Left)]) {
        rotateFactor += 30.0f * deltaTime ;
    }

    if (m_actionData.m_input[static_cast<size_t>(Action::Right)]) {
        rotateFactor -= 30.0f * deltaTime ;
    }

    if (m_actionData.m_input[static_cast<size_t>(Action::Forward)]) {
        updateDirection(rotateFactor);
        m_position += m_forward * m_speed * deltaTime;
    } 
    
    if (m_actionData.m_input[static_cast<size_t>(Action::Backward)]) {
        rotateFactor *= -1.0f;
        updateDirection(rotateFactor);
        m_position += m_forward * m_speed * deltaTime * (-1.0f);
    }
    m_modelMatrix = glm::translate(glm::mat4{1.0f}, m_position);
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotationAngle), glm::vec3(0, 1, 0));    
}

void Vehicle::setAction(Action action) {
    m_actionData.m_input.set(static_cast<size_t>(action));
}

void Vehicle::resetAction(Action action) {
    m_actionData.m_input.reset(static_cast<size_t>(action));
}

void Vehicle::updateDirection(float rotateFactor) {
    if (rotateFactor == 0) return;
    m_rotationAngle += rotateFactor;

    float x = m_forward.x;
    float z = m_forward.z;

    m_forward.z = z * glm::cos(glm::radians(rotateFactor)) - x *  glm::sin(glm::radians(rotateFactor)) ;
    m_forward.x = z * glm::sin(glm::radians(rotateFactor)) + x *  glm::cos(glm::radians(rotateFactor)) ;

}