cmake_minimum_required (VERSION 3.8)

project("gui")

add_library(${PROJECT_NAME} window.h window.cpp
				            button.h button.cpp "keyboard.cpp" "mouse.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ../)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
