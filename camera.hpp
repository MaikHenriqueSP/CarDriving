#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "vehicle.hpp"

class OpenGLWindow;

class Camera {
 public:
  void computeViewMatrix(); // Matriz de visão
  void computeProjectionMatrix(int width, int height); // Matriz de projeção

  void dolly();
  void truck(float speed);
  void pan(float speed);
  void setVehicle(Vehicle* vehicle);

 private:
  friend OpenGLWindow;

  Vehicle* m_vehicle;
  glm::vec3 m_eye{glm::vec3(0.0f, 0.6f, 2.5f)};  // Camera position
  glm::vec3 m_at{glm::vec3(0.0f, 0.6f, 0.0f)};   // Look-at point
  glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};   // "up" direction

  // Matrix to change from world space to camera soace
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;
};

#endif