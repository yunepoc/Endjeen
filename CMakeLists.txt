cmake_minimum_required(VERSION 3.0)

project(Endjeen)

file(
  GLOB_RECURSE
  source
  main.cpp
  src/*.cpp
)

include_directories(include)
add_executable(Endjeen ${source})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(Endjeen ${GLEW_LIBRARIES})

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(Endjeen glfw)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(Endjeen ${OPENGL_LIBRARIES})
