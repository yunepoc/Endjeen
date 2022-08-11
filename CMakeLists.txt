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
