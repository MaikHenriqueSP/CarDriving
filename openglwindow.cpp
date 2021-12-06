#include "openglwindow.hpp"
#include <imgui.h>
#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  switch (ev.key.keysym.sym) {
    case SDLK_UP:
    case SDLK_w:
      m_carModel.updateAction(ev.type, Action::Forward);
      break;
    case SDLK_DOWN:
    case SDLK_s:
      m_carModel.updateAction(ev.type, Action::Backward);
      break;
    case SDLK_LEFT:
    case SDLK_a:
      m_carModel.updateAction(ev.type, Action::Left);
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      m_carModel.updateAction(ev.type, Action::Right);
      break;
  }  
}

void OpenGLWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);

  auto path{getAssetsPath() + "shaders/" + m_shaderName};
  m_program = createProgramFromFile(path + ".vert", path + ".frag");

  loadModel("formula_1_mesh.obj", "maps/formula1_DefaultMaterial_Diffuse.png", &m_carModel);
  m_carModel.m_modelMatrix =   glm::translate(m_carModel.m_modelMatrix, glm::vec3(0.0f, 0.0f, -0.5f));
  m_carModel.m_modelMatrix = glm::rotate(m_carModel.m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));

  loadModel("road.obj", "maps/road.jpg", &m_roadModel);
  m_roadModel.m_modelMatrix =  glm::translate(m_roadModel.m_modelMatrix, glm::vec3(0.0f, -0.25f, -1.0f));
  m_roadModel.m_modelMatrix = glm::rotate(m_roadModel.m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  m_roadModel.m_modelMatrix = glm::scale(m_roadModel.m_modelMatrix, glm::vec3(35.0f, 1.0f, 35.0f));

  auto wallObjectFile = "box.obj";
  auto wallMapFile = "maps/cube.png";

  loadModel(wallObjectFile, wallMapFile, &m_leftWallModel);
  m_leftWallModel.m_modelMatrix =  glm::translate(m_leftWallModel.m_modelMatrix, glm::vec3(m_leftLimit, 0.0f, -1.0f));
  m_leftWallModel.m_modelMatrix = glm::scale(m_leftWallModel.m_modelMatrix, glm::vec3(0.0f, 1.0f, 60.0f));

  loadModel(wallObjectFile, wallMapFile, &m_rightWallModel);
  m_rightWallModel.m_modelMatrix =  glm::translate(m_rightWallModel.m_modelMatrix, glm::vec3(m_rightLimit, 0.0f, -1.0f));
  m_rightWallModel.m_modelMatrix = glm::scale(m_rightWallModel.m_modelMatrix, glm::vec3(0.0f, 1.0f, 60.0f));

  loadModel(wallObjectFile, wallMapFile, &m_frontWallModel);
  m_backWallModel.m_modelMatrix =  glm::translate(m_backWallModel.m_modelMatrix, glm::vec3(0.0f, 0.0f, m_backwardLimit));
  m_backWallModel.m_modelMatrix = glm::rotate(m_backWallModel.m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  m_backWallModel.m_modelMatrix = glm::scale(m_backWallModel.m_modelMatrix, glm::vec3(0.0f, 1.0f, 10.0f));

  loadModel(wallObjectFile, wallMapFile, &m_backWallModel);
  m_frontWallModel.m_modelMatrix = glm::translate(m_frontWallModel.m_modelMatrix, glm::vec3(0.0f, 0.0f, m_frontLimit));
  m_frontWallModel.m_modelMatrix = glm::rotate(m_frontWallModel.m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  m_frontWallModel.m_modelMatrix = glm::scale(m_frontWallModel.m_modelMatrix, glm::vec3(0.0f, 1.0f, 10.0f));

  m_carModel.setBorderLimit(BorderLimits::Backward, m_backwardLimit);
  m_carModel.setBorderLimit(BorderLimits::Front, m_frontLimit);
  m_carModel.setBorderLimit(BorderLimits::Left, m_leftLimit);
  m_carModel.setBorderLimit(BorderLimits::Right, m_rightLimit);

  resizeGL(getWindowSettings().width, getWindowSettings().height);
  m_camera.initialize(&m_carModel);

}

void OpenGLWindow::loadModel(std::string objectPath, std::string texturePath, Model* model) {
  auto textureFullPath = getAssetsPath() + texturePath;
  auto objectFullPath = getAssetsPath() + objectPath;
  model->loadDiffuseTexture(textureFullPath);
  model->loadFromFile(objectFullPath);
  model->setupVAO(m_program);
}

void OpenGLWindow::paintGL() {
  float deltaTime{static_cast<float>(getDeltaTime())};
    
  update(deltaTime);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);

  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};
  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};
  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};

  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform1i(diffuseTexLoc, 0);

  auto lightDirRotated{m_lightDir};
  glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);

  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);

  configureModel(&m_carModel);
  configureModel(&m_roadModel);
  configureModel(&m_leftWallModel);
  configureModel(&m_rightWallModel);
  configureModel(&m_frontWallModel);
  configureModel(&m_backWallModel);

  glUseProgram(0);
}

void OpenGLWindow::configureModel(Model* model) {
  const auto program{m_program};
  auto modelMatrixLoc{glGetUniformLocation(program, "modelMatrix")};
  auto shininessLoc{glGetUniformLocation(program, "shininess")};
  auto normalMatrixLoc{glGetUniformLocation(program, "normalMatrix")};
  auto KaLoc{glGetUniformLocation(program, "Ka")};
  auto KdLoc{glGetUniformLocation(program, "Kd")};
  auto KsLoc{glGetUniformLocation(program, "Ks")};

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model->m_modelMatrix[0][0]);
  auto modelViewMatrixRoad{glm::mat3(m_camera.m_viewMatrix * model->m_modelMatrix)};
  glm::mat3 normalMatrixRoad{glm::inverseTranspose(modelViewMatrixRoad)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrixRoad[0][0]);

  auto modelKa = model->getKa();
  auto modelKd = model->getKd();
  auto modelKs = model->getKs();
  
  glUniform1f(shininessLoc, model->getShininess());
  glUniform4fv(KaLoc, 1, &modelKa.x);
  glUniform4fv(KdLoc, 1, &modelKd.x);
  glUniform4fv(KsLoc, 1, &modelKs.x);
  model->render();
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() { glDeleteProgram(m_program); }

void OpenGLWindow::update(float deltaTime) {
  m_camera.update();
  m_carModel.update(deltaTime);
}