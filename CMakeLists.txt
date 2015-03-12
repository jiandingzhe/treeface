cmake_minimum_required(VERSION 3.0)
project(treeface)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(GLEW REQUIRED)

message(STATUS "GLEW")
message(STATUS "  lib ${GLEW_LIBRARIES}")
message(STATUS "  inc ${GLEW_INCLUDE_DIRS}")

include_directories(${GLEW_INCLUDE_DIRS})

add_subdirectory(test)

