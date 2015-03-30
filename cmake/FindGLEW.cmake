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

set(GLEW_SEARCH_PREFIX      "" CACHE PATH   "additional path to search for GLEW")
set(GLEW_SEARCH_PREFIX_INC  "" CACHE PATH   "additional path to search for GLEW header GL/glew.h")
set(GLEW_SEARCH_PREFIX_LIB  "" CACHE PATH   "additional path to search for GLEW libraries")
set(GLEW_SEARCH_STATIC      NO CACHE BOOL   "search for static library")
set(GLEW_SEARCH_MX          NO CACHE BOOL   "search for multi-context version")
set(GLEW_SEARCH_32BIT_64BIT 64 CACHE STRING "search for 32bit or 64bit library")

#
# find header directory
find_path(GLEW_INCLUDE_DIR GL/glew.h
    HINTS ${GLEW_SEARCH_PREFIX_INC} ${GLEW_SEARCH_PREFIX}/include
)

#
# find library
#

# set library name and path search hint, according to expected type
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(GLEW_SEARCH_MX)
        if(GLEW_SEARCH_32BIT_64BIT EQUAL 32)
            set(GLEW_LIB_HINT
                "${GLEW_SEARCH_PREFIX}/lib/Release MX/Win32"
                ${GLEW_SEARCH_PREFIX}/lib
                ${GLEW_SEARCH_PREFIX}
            )
        elseif(GLEW_SEARCH_32BIT_64BIT EQUAL 64)
            set(GLEW_LIB_HINT
                "${GLEW_SEARCH_PREFIX}/lib/Release MX/x64"
                ${GLEW_SEARCH_PREFIX}/lib
                ${GLEW_SEARCH_PREFIX}
            )
        else()
            message(FATAL_ERROR "invalid value for GLEW_SEARCH_32BIT_64BIT: ${GLEW_SEARCH_32BIT_64BIT}, only 32|64 allowed.")
        endif()

        if(GLEW_SEARCH_STATIC)
            set(_glew_lib_name_ "glew32mxs")
        else()
            set(_glew_lib_name_ "glew32mx")
        endif()
    else()
        if(GLEW_SEARCH_32BIT_64BIT EQUAL 32)
            set(GLEW_LIB_HINT
                "${GLEW_SEARCH_PREFIX}/lib/Release/Win32"
                ${GLEW_SEARCH_PREFIX}/lib
                ${GLEW_SEARCH_PREFIX}
            )
        elseif(GLEW_SEARCH_32BIT_64BIT EQUAL 64)
            set(GLEW_LIB_HINT
                "${GLEW_SEARCH_PREFIX}/lib/Release/x64"
                ${GLEW_SEARCH_PREFIX}/lib
                ${GLEW_SEARCH_PREFIX}
            )
        else()
            message(FATAL_ERROR "invalid value for GLEW_SEARCH_32BIT_64BIT: ${GLEW_SEARCH_32BIT_64BIT}, only 32|64 allowed.")
        endif()

        if(GLEW_SEARCH_STATIC)
            set(_glew_lib_name_ "glew32s")
        else()
            set(_glew_lib_name_ "glew32")
        endif()
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(GLEW_LIB_HINT ${GLEW_SEARCH_PREFIX}/lib)

    if(GLEW_SEARCH_MX)
        set(_glew_lib_name_ "GLEWmx")
    else()
        set(_glew_lib_name_ "GLEW")
    endif()

    if(GLEW_SEARCH_STATIC)
        set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
else()
    message(FATAL_ERROR "unsupported system")
endif()

# do search lib
find_library(GLEW_LIBRARY
    NAMES ${_glew_lib_name_}
    HINTS ${GLEW_SEARCH_PREFIX_LIB} ${GLEW_LIB_HINT}
)

#
# finalize
#
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW
    REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR
    VERSION_VAR GLEW_VERSION
)
