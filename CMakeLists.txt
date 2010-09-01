cmake_minimum_required(VERSION 2.6)

project(OAuth2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
include_directories("include")
add_subdirectory(src)
add_subdirectory(test)
