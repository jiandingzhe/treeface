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

set(GLEW_SEARCH_PREFIX     "" CACHE PATH "additional path to search for GLEW")
set(GLEW_SEARCH_PREFIX_INC "" CACHE PATH "additional path to search for GLEW header GL/glew.h")
set(GLEW_SEARCH_PREFIX_LIB "" CACHE PATH "additional path to search for GLEW libraries")
set(GLEW_SEARCH_STATIC     NO CACHE BOOL "search for static GLEW library")
set(GLEW_SEARCH_MX         NO CACHE BOOL "search for multi context GLEW library")

# find header directory
find_path(GLEW_INCLUDE_DIR GL/glew.h
    HINTS ${GLEW_SEARCH_PREFIX_INC} ${GLEW_SEARCH_PREFIX}
    PATH_SUFFIXES GLEW
)

# find library

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	if(GLEW_SEARCH_STATIC)
		if(GLEW_SEARCH_MX)
			set(_glew_lib_name_ "glew32mxs")
		else()
			set(_glew_lib_name_ "glew32s")
		endif()
	else()
		if(GLEW_SEARCH_MX)
			set(_glew_lib_name_ "glew32mx")
		else()
			set(_glew_lib_name_ "glew32")
		endif()
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(_glew_lib_name_ "GLEW")
	if(GLEW_SEARCH_STATIC)
		set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
	endif()
else()
	message(FATAL_ERROR "unsupported system")
endif()

find_library(GLEW_LIBRARY
	NAMES ${_glew_lib_name_}
	HINTS ${GLEW_SEARCH_PREFIX_LIB} ${GLEW_SEARCH_PREFIX}
)

# finalize
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW
    REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR
    VERSION_VAR GLEW_VERSION
)
