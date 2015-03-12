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

# find header directory
find_path(SDL2_INCLUDE_DIR SDL.h
    HINTS ${SDL2_FIND_PREFIX}
    PATH_SUFFIXES SDL2
)

# read version from header file content
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

# find library
if(SDL2_FIND_STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

find_library(SDL2_LIBRARY SDL2
    HINTS ${SDL2_FIND_PREFIX}
)

# finalize
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2
    REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR
    VERSION_VAR SDL2_VERSION
)
