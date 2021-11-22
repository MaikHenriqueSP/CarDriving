project(CarDriving)
add_executable(${PROJECT_NAME} main.cpp model.cpp openglwindow.cpp camera.cpp vehicle.cpp)
enable_abcg(${PROJECT_NAME})
