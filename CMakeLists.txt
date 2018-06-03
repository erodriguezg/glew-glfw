# PAQUETES UBUNTU:
#OPENGL
#sudo apt install mesa-utils
#GLEW
#sudo apt-get install libglew-dev
#GL MATH
#sudo apt-get install libglm-dev
#GLFW
#sudo apt-get install libglfw3-dev

cmake_minimum_required(VERSION 3.5.1)
project(glew-glfw)

set(CMAKE_CXX_STANDARD 11)

add_executable(glew-glfw main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

target_link_libraries(glew-glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)

#copy resources
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

