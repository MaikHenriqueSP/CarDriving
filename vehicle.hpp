#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include "model.hpp"
#include "actiondata.hpp"

class Vehicle : public Model {
    public:
        void update(float deltaTime);
        void setAction(Action action);
        void resetAction(Action action);
    private:
        float m_speed{1.0f};
        glm::vec3 m_forward{0.0f, 0.0f, -1.0f};
        glm::vec3 m_position{0.0f, 0.0f, -0.5f};
        ActionData m_actionData;
        float m_rotationAngle{-90.0f};
        void updateDirection(float rotateFactor);

};

#endif