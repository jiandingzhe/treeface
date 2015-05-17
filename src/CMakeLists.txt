#
# configure file
#

# GL version macro
if(TREEFACE_GL_VERSION STREQUAL "3.0")
	set(TREEFACE_GL_MACRO_SUFFIX "3_0")
elseif(TREEFACE_GL_VERSION STREQUAL "3.1")
	set(TREEFACE_GL_MACRO_SUFFIX "3_1")
elseif(TREEFACE_GL_VERSION STREQUAL "3.2")
	set(TREEFACE_GL_MACRO_SUFFIX "3_2")
elseif(TREEFACE_GL_VERSION STREQUAL "3.3")
	set(TREEFACE_GL_MACRO_SUFFIX "3_3")
elseif(TREEFACE_GL_VERSION STREQUAL "4.0")
	set(TREEFACE_GL_MACRO_SUFFIX "4_0")
elseif(TREEFACE_GL_VERSION STREQUAL "ES 3.0")
	set(TREEFACE_GL_MACRO_SUFFIX "ES_3_0")
elseif(TREEFACE_GL_VERSION STREQUAL "ES 3.1")
	set(TREEFACE_GL_MACRO_SUFFIX "ES_3_1")
else()
	message(FATAL_ERROR "unsupported OpenGL version ${TREEFACE_GL_VERSION}")
endif()

configure_file(treeface/config.h.in treeface/config.h @ONLY)

#
# treeface
#
file(GLOB treeface_src_files
    "treeface/*.h"
    "treeface/*.cpp"
    "treeface/*/*.h"
    "treeface/*/*.cpp"
)

set(nedmalloc_src_files nedmalloc.h nedmalloc.cpp malloc.c.h)

add_library(treeface STATIC
    ${nedmalloc_src_files}
    ${treeface_src_files}
)




