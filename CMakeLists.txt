# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("wui")

# Include sub-projects.
add_subdirectory ("wui")

#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
