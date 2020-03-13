# CMakeList.txt : CMake project for projectY, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
	add_compile_definitions(DEBUG=1)
endif(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} WIN32 WinMain.cpp)

# TODO: Add tests and install targets if needed.


add_subdirectory(gui)
include_directories(gui)
target_link_libraries(${PROJECT_NAME} gui)
target_include_directories(${PROJECT_NAME} PUBLIC gui)

add_subdirectory(containers)
include_directories(containers)
target_link_libraries(${PROJECT_NAME} containers)
target_include_directories(${PROJECT_NAME} PUBLIC containers)

add_subdirectory(util)
include_directories(util)
target_link_libraries(${PROJECT_NAME} util)
target_include_directories(${PROJECT_NAME} PUBLIC util)

target_link_libraries(${PROJECT_NAME} d3d11.lib)
