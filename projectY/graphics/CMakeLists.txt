cmake_minimum_required (VERSION 3.8)

project("graphics")

add_library(${PROJECT_NAME} vertex_buffer.h vertex_buffer.cpp
							renderer.h		renderer.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../)
target_include_directories(${PROJECT_NAME} PUBLIC ../gui)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
