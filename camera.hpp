#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "vehicle.hpp"

class OpenGLWindow;

class Camera {
 public:
  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height); 
  void dolly();
  void pan(float deltaTime);
  void initialize(Vehicle* vehicle);
  
 private:
  friend OpenGLWindow;
  Vehicle* m_vehicle;
  const float m_zoom{2.5f};
  glm::vec3 m_eye{glm::vec3(0.0f, 0.6f, 2.5f)};
  glm::vec3 m_at;
  glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};
  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;
};

#endif