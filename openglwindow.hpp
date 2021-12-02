#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <string_view>

#include "abcg.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "vehicle.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  int m_viewportWidth{};
  int m_viewportHeight{};

  Vehicle m_carModel;
  Model m_roadModel;
  Model m_leftWallModel;
  Model m_rightWallModel;
  Model m_frontWallModel;
  Model m_backWallModel;

  int m_trianglesToDraw{};

  const float m_leftLimit{-4.75f};
  const float m_rightLimit{4.75f};
  const float m_backwardLimit{-35.7f};
  const float m_frontLimit{33.7f};
  
  const char* m_shaderName{"texture"};
  GLuint m_program;
  
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{0.549f, 0.516f, 0.516f, 1.0f};
  glm::vec4 m_Id{0.797f, 0.767f, 0.767f, 1.0f};
  glm::vec4 m_Is{1.0f};
  
  void loadModel(std::string objectPath, std::string texturePath, Model* model);
  void update();
  void configureModel(Model* model);

  Camera m_camera;
};

#endif