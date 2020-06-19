cmake_minimum_required (VERSION 3.8)

project("yml")

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} lexer.h lexer.cpp
                            token.h "namespace.h")
