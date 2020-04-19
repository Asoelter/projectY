# CMakeList.txt : CMake project for projectY, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
	add_compile_definitions(DEBUG=1)
endif(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} WIN32 WinMain.cpp "graphics/vertex_buffer.hpp" "graphics/renderer.cpp")

add_subdirectory(gui)
include_directories(gui)
target_link_libraries(${PROJECT_NAME} gui)
target_include_directories(${PROJECT_NAME} PUBLIC gui)

add_subdirectory(graphics)
include_directories(graphics)
target_link_libraries(${PROJECT_NAME} graphics)
target_include_directories(${PROJECT_NAME} PUBLIC graphics)

add_subdirectory(containers)
include_directories(containers)
target_link_libraries(${PROJECT_NAME} containers)
target_include_directories(${PROJECT_NAME} PUBLIC containers)

add_subdirectory(util)
include_directories(util)
target_link_libraries(${PROJECT_NAME} util)
target_include_directories(${PROJECT_NAME} PUBLIC util)

target_link_libraries(${PROJECT_NAME} d3d11.lib)
target_link_libraries(${PROJECT_NAME} D3DCompiler.lib)
