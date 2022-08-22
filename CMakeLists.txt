cmake_minimum_required(VERSION 3.0)

project(Endjeen)

file(
  GLOB_RECURSE
  source
  main.cpp
  src/*.cpp
  external/*.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

include_directories(include)
include_directories(external)
add_executable(Endjeen ${source})

# Assimp
find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIR})
target_link_libraries(Endjeen ${ASSIMP_LIBRARIES})

# Bullet
find_package(Bullet REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})
target_link_libraries(Endjeen ${BULLET_LIBRARIES})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(Endjeen ${GLEW_LIBRARIES})

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(Endjeen glfw)

# GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(Endjeen ${OPENGL_LIBRARIES})
