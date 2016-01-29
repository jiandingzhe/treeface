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

configure_file(treeface/Config.h.in treeface/Config.h @ONLY)

#
# treeface
#
file(GLOB_RECURSE treeface_src_files "treeface/*.h" "treeface/*.cpp")

add_library(treeface STATIC
    ${treeface_src_files}
    ${PROJECT_SOURCE_DIR}/uncrustify.cfg
)
target_use_treecore(treeface)

#
# a strange macro for 2D graphic stepwise visualise debug
#
set(TREECORE_2D_GEOMETRY_STEPWISE_VISUAL 0 CACHE BOOL "")
if(TREECORE_2D_GEOMETRY_STEPWISE_VISUAL)
    find_package(PkgConfig)
    pkg_check_modules(CAIRO REQUIRED cairo)
    add_definitions(-DSUCK_TREECORE_GEOMETRY)
    target_include_directories(treeface PUBLIC ${CAIRO_INCLUDE_DIRS})
    target_link_libraries(treeface ${CAIRO_LIBRARIES})
endif()


#
# utilities
#

add_executable(gen_iter_sphere gen_iter_sphere.cpp)
target_link_libraries(gen_iter_sphere treeface)
target_use_treecore(gen_iter_sphere)
