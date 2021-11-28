#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include "actiondata.hpp"
#include "model.hpp"

class Vehicle : public Model {
 public:
  void update(float deltaTime);
  float getSpeed();
  glm::vec3 getPosition();
  float getRotationFactor(float deltaTime);
  glm::vec3 getForward();
  void updateAction(Uint32 eventType, Action action);
 private:
  float m_speed{0.0f};
  glm::vec3 m_forward{0.0f, 0.0f, -1.0f};
  glm::vec3 m_position{0.0f, 0.0f, -0.5f};
  ActionData m_actionData;
  float m_rotationAngle{-90.0f};
  void updateDirection(float rotateFactor);
  void updateSpeed(float deltaTime);
};

#endif