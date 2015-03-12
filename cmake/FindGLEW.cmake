#.rst:
# FindGLEW
# --------
#
# Locate GLEW library
#
# This Module defines:
#
# ::
#
#   GLEW_FOUND: whether GLEW exists
#   GLEW_LIBRARY: GLEW library file
#   GLEW_INCLUDE_DIR: the directory containing GL/glew.h
#
# Before calling FIND_PACKAGE(GLEW), you may set these variabels for some hint:
#
# ::
#
#   GLEW_FIND_PREFIX: this path will be searched in prior to system paths
#   GLEW_FIND_STATIC: if set to true, the static library will be searched;
#                     otherwise dynamic library will be searched

include(FindPackageMessage)
include(FindPackageHandleStandardArgs)

set(GLEW_FIND_PREFIX "" CACHE PATH "additional path to search for GLEW")
set(GLEW_FIND_STATIC NO CACHE BOOL "search for static GLEW library")

# find header directory
find_path(GLEW_INCLUDE_DIR GL/glew.h
    HINTS ${GLEW_FIND_PREFIX}
    PATH_SUFFIXES GLEW
)

# find library
if(GLEW_FIND_STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

find_library(GLEW_LIBRARY GLEW
    HINTS ${GLEW_FIND_PREFIX}
)

# finalize
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW
    REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR
    VERSION_VAR GLEW_VERSION
)
