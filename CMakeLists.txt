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
message(STATUS "treejuce: ${TREEJUCE_FOUND}")
message(STATUS "  lib ${TREEJUCE_LIBRARIES}")
message(STATUS "  inc ${TREEJUCE_INCLUDE_DIR}")

find_package(OpenGL REQUIRED)
message(STATUS "GL: ${OPENGL_FOUND}")
message(STATUS "GLU: ${OPENGL_GLU_FOUND}")
message(STATUS "  lib ${OPENGL_gl_LIBRARY}")
message(STATUS "  inc ${OPENGL_INCLUDE_DIR}")

find_package(GLEW REQUIRED)
message(STATUS "GLEW: ${GLEW_FOUND}")
message(STATUS "  lib ${GLEW_LIBRARY}")
message(STATUS "  inc ${GLEW_INCLUDE_DIR}")

find_package(FreeImage REQUIRED)
message(STATUS "FreeImage: ${FreeImage_FOUND}")
message(STATUS "  lib ${FreeImage_LIBRARIES}")
message(STATUS "  inc ${FreeImage_INCLUDE_DIR}")

find_package(SDL2)
message(STATUS "SDL2: ${SDL2_FOUND}")
message(STATUS "  inc ${SDL2_INCLUDE_DIR}")
message(STATUS "  lib ${SDL2_LIBRARY}")

find_package(COMPRESSOR)
message(STATUS "compressor: ${COMPRESSOR_FOUND}")
message(STATUS "  bin: ${COMPRESSOR_EXECUTABLE}")

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

#if(TREEFACE_OS STREQUAL "WINDOWS")
#    add_definitions(-DJUCE_DLL)
#endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${GLEW_INCLUDE_DIR}
    ${TREEJUCE_INCLUDE_DIR}
    ${FreeImage_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)

if(COMPRESSOR_FOUND)
    add_subdirectory(examples)
endif()

if(SDL2_FOUND AND COMPRESSOR_FOUND)
    add_subdirectory(test)
    add_subdirectory(t)
endif()

add_subdirectory(src)
