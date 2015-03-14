cmake_minimum_required(VERSION 3.0)
project(treeface)

set(TREEFACE_VERSION_MAJOR 0)
set(TREEFACE_VERSION_MINOR 0)
set(TREEFACE_VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(GLEW REQUIRED)

message(STATUS "GLEW")
message(STATUS "  lib ${GLEW_LIBRARY}")
message(STATUS "  inc ${GLEW_INCLUDE_DIR}")

add_definitions(-std=c++11)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${GLEW_INCLUDE_DIR}
)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(t)
