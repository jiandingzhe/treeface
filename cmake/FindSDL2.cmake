#.rst:
# FindSDL2
# --------
#
# Locate SDL2 library
#
# This Module defines:
#
# ::
#
#   SDL2_FOUND: whether SDL2 exists
#   SDL2_LIBRARY: SDL2 library file
#   SDL2_INCLUDE_DIR: the directory containing SDL.h
#   SDL2_VERSION
#   SDL2_VERSION_MAJOR
#   SDL2_VERSION_MINOR
#   SDL2_VERSION_PATCH: versions for SDL2 library, which is parsed from the
#                       contents in "SDL_version.h".
#
# Before calling FIND_PACKAGE(SDL2), you may set these variabels for some hint:
#
# ::
#
#   SDL2_FIND_PREFIX: this path will be searched in prior to system paths
#   SDL2_FIND_STATIC: if set to true, the static library will be searched;
#                     otherwise dynamic library will be searched

include(FindPackageMessage)
include(FindPackageHandleStandardArgs)

set(SDL2_SEARCH_PREFIX      "" CACHE PATH   "additional path to search for SDL2")
set(SDL2_SEARCH_PREFIX_INC  "" CACHE PATH   "additional path to search for SDL2 header SDL.h")
set(SDL2_SEARCH_PREFIX_LIB  "" CACHE PATH   "additional path to search for SDL2 library")
set(SDL2_SEARCH_STATIC      NO CACHE BOOL   "search for static SDL2 library")
set(SDL2_SEARCH_32BIT_64BIT 64 CACHE STRING "search for 32bit or 64bit library")

#
# find header directory
#
find_path(SDL2_INCLUDE_DIR SDL.h
    HINTS ${SDL2_SEARCH_PREFIX_INC} ${SDL2_SEARCH_PREFIX}/include
    PATH_SUFFIXES SDL2
)

#
# read version from header file content
#
if(SDL2_INCLUDE_DIR)
    file(STRINGS ${SDL2_INCLUDE_DIR}/SDL_version.h tmp1 REGEX "define +SDL_MAJOR_VERSION +[0-9]+")
    file(STRINGS ${SDL2_INCLUDE_DIR}/SDL_version.h tmp2 REGEX "define +SDL_MINOR_VERSION +[0-9]+")
    file(STRINGS ${SDL2_INCLUDE_DIR}/SDL_version.h tmp3 REGEX "define +SDL_PATCHLEVEL +[0-9]+")

    if(NOT tmp1 OR NOT tmp2 OR NOT tmp3)
        message(FATAL_ERROR "failed to parse version from SDL heade file ${SDL2_INCLUDE_DIR}/SDL_version.h")
    endif()

    string(REGEX MATCH "[0-9]+" SDL2_VERSION_MAJOR ${tmp1})
    string(REGEX MATCH "[0-9]+" SDL2_VERSION_MINOR ${tmp2})
    string(REGEX MATCH "[0-9]+" SDL2_VERSION_PATCH ${tmp3})
    set(SDL2_VERSION ${SDL2_VERSION_MAJOR}.${SDL2_VERSION_MINOR}.${SDL2_VERSION_PATCH})
endif()

#
# find library
#
if(SDL2_FIND_STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

# set library path to fit different path layouts on different OS
if(WIN32)
    if(SDL2_SEARCH_32BIT_64BIT EQUAL 32)
        set(SDL2_LIB_HINT ${SDL2_SEARCH_PREFIX}/lib/x86 ${SDL2_SEARCH_PREFIX}/lib ${SDL2_SEARCH_PREFIX})
    elseif(SDL2_SEARCH_32BIT_64BIT EQUAL 64)
        set(SDL2_LIB_HINT ${SDL2_SEARCH_PREFIX}/lib/x64 ${SDL2_SEARCH_PREFIX}/lib ${SDL2_SEARCH_PREFIX})
    else()
        message(FATAL_ERROR "invalid value for SDL2_SEARCH_32BIT_64BIT: ${SDL2_SEARCH_32BIT_64BIT}, only 32|64 allowed.")
    endif()
else()
    set(SDL2_LIB_HINT ${SDL2_SEARCH_PREFIX}/lib ${SDL2_SEARCH_PREFIX})
endif()

find_library(SDL2_LIBRARY SDL2
    HINTS ${SDL2_SEARCH_PREFIX_LIB} ${SDL2_LIB_HINT}
)

# finalize
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2
    REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR
    VERSION_VAR SDL2_VERSION
)
