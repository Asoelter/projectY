cmake_minimum_required (VERSION 3.8)

project("math")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/vec3.h)
