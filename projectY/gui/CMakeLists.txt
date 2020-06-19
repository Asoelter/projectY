cmake_minimum_required (VERSION 3.8)

project("gui")

add_library(${PROJECT_NAME} window.h	window.cpp
				            button.h	button.cpp 
							keyboard.h	keyboard.cpp 
							mouse.h		mouse.cpp 
							menu.h		menu.cpp "gui_element.h" "namespace.h")

target_include_directories(${PROJECT_NAME} PUBLIC ../)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
