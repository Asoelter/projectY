cmake_minimum_required (VERSION 3.8)

project("graphics")

add_library(${PROJECT_NAME} vertex_buffer.h vertex_buffer.hpp
							renderer.h      renderer.cpp 
							vertex_shader.h vertex_shader.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../)
target_include_directories(${PROJECT_NAME} PUBLIC ../gui)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
