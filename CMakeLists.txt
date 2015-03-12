cmake_minimum_required(VERSION 3.0)
project(treeface)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(GLEW REQUIRED)

message(STATUS "GLEW")
message(STATUS "  lib ${GLEW_LIBRARY}")
message(STATUS "  inc ${GLEW_INCLUDE_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/src ${GLEW_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(t)
