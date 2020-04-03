cmake_minimum_required (VERSION 3.8)

project("util")

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} uuid.h uuid.cpp 
							ywin.h error_printer.h)

target_link_libraries(${PROJECT_NAME} rpcrt4.lib)

#target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ywin.h)
