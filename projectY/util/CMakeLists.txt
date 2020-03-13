cmake_minimum_required (VERSION 3.8)

project("util")

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ywin.h)
