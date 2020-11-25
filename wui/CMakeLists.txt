# CMakeList.txt : CMake project for projectY, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
	add_compile_definitions(DEBUG=1)
endif(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")

add_compile_definitions(NOMINMAX=1)
add_compile_definitions(WIN32_LEAN_AND_MEAN=1)

# Add source to this project's executable.
add_library (${PROJECT_NAME} INTERFACE)

function(addDirectory dir)
	add_subdirectory(${dir})
	include_directories(${dir})
	target_link_libraries(${PROJECT_NAME} INTERFACE ${dir})
	target_include_directories(${PROJECT_NAME} INTERFACE ${dir})
endfunction()

addDirectory(containers)
addDirectory(graphics)
addDirectory(gui)
addDirectory(math)
addDirectory(util)

target_link_libraries(${PROJECT_NAME} INTERFACE d3d11.lib)
target_link_libraries(${PROJECT_NAME} INTERFACE D3DCompiler.lib)
