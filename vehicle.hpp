#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include "actiondata.hpp"
#include "model.hpp"
#include "borderlimits.hpp"
#include <unordered_map>

class Vehicle : public Model {
 public:
  float getSpeed();
  float getRotationFactor(float deltaTime);
  void update(float deltaTime);
  void updateAction(Uint32 eventType, Action action);
  void setBorderLimit(BorderLimits border, float location);
  glm::vec3 getPosition();
  glm::vec3 getForward();

 private:
  float m_speed{0.0f};
  glm::vec3 m_forward{0.0f, 0.0f, -1.0f};
  glm::vec3 m_position{0.0f, 0.0f, -0.5f};
  std::unordered_map<BorderLimits, float> m_borderLimitsMap{};
  ActionData m_actionData;
  float m_rotationAngle{-90.0f};
  float m_scaleFactor{0.55f};
  const float m_baseSteeringAngle{30.0f};
  
  void updateDirection(float rotateFactor);
  void updateSpeed(float deltaTime);
  void updateOffLimits();
};

#endif