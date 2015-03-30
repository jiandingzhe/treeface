cmake_minimum_required(VERSION 3.0)
project(treeface)

set(TREEFACE_VERSION_MAJOR 0)
set(TREEFACE_VERSION_MINOR 0)
set(TREEFACE_VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# unify processor name
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set(TREEFACE_CPU "X86_64")
else()
	message(FATAL_ERROR "unsupported processor: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# unify os name
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(TREEFACE_OS "WINDOWS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(TREEFACE_OS "LINUX")
else()
	message(FATAL_ERROR "unsupported target OS: ${CMAKE_SYSTEM_NAME}")
endif()

#
# find prerequisite libraries
#
find_package(TREEJUCE REQUIRED)
message(STATUS "treejuce")
message(STATUS "  inc ${TREEJUCE_INCLUDE_DIR}")
message(STATUS "  lib ${TREEJUCE_LIBRARIES}")

find_package(OpenGL REQUIRED)

message(STATUS "GL")
message(STATUS "  lib ${OPENGL_gl_LIBRARY}")

find_package(GLEW REQUIRED)

message(STATUS "GLEW")
message(STATUS "  lib ${GLEW_LIBRARY}")
message(STATUS "  inc ${GLEW_INCLUDE_DIR}")

#
# set compiler options
#
if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
	add_definitions(-std=c++11)
	if(TREEFACE_CPU STREQUAL "X86_64")
		add_definitions(-msse2)
	else()
	endif()
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${GLEW_INCLUDE_DIR}
    ${TREEJUCE_INCLUDE_DIR}
)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(t)
