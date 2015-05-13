include(FindPackageMessage)
include(FindPackageHandleStandardArgs)
include(GNUInstallDirs)

set(TREEJUCE_SEARCH_PREFIX "" CACHE PATH "additional path to search for treejuce")
set(TREEJUCE_SEARCH_PREFIX_BIN "" CACHE PATH "additional path to search for treejuce programs")
set(TREEJUCE_SEARCH_PREFIX_INC "" CACHE PATH "additional path to search for treejuce headers")
set(TREEJUCE_SEARCH_PREFIX_LIB "" CACHE PATH "additional path to search for treejuce library")
set(TREEJUCE_SEARCH_STATIC 0 CACHE BOOL "search static library instead of shared library")

#
# find binary builder
#
find_program(TREEJUCE_BIN_BUILDER treejuce_bin_builder
    HINTS
        ${TREEJUCE_SEARCH_PREFIX_BIN}
        ${TREEJUCE_SEARCH_PREFIX}/${CMAKE_INSTALL_BINDIR}
        ${TREEJUCE_SEARCH_PREFIX}/bin
)

function(TREEJUCE_WRAP_RESOURCE input_dir class_name)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/${class_name}.h
            ${CMAKE_CURRENT_BINARY_DIR}/${class_name}.cpp
        COMMAND
            ${TREEJUCE_BIN_BUILDER} ${input_dir} ${CMAKE_CURRENT_BINARY_DIR} ${class_name}
    )
endfunction()

#
# find header directory
#

find_path(TREEJUCE_INCLUDE_DIR treejuce/Config.h
    HINTS
        ${TREEJUCE_SEARCH_PREFIX_INC}
        ${TREEJUCE_SEARCH_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
        ${TREEJUCE_SEARCH_PREFIX}/include
)

#
# find library
#

if(WIN32)
    set(treejuce_dep_libs version winmm Shlwapi Dbghelp)
elseif(UNIX)
    set(treejuce_dep_libs pthread dl)
endif()

if(TREEJUCE_SEARCH_STATIC)
    set(treejuce_lib_name "treejuce_s")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set(treejuce_lib_name "treejuce")
endif()

if(NOT TREEJUCE_LIBRARIES)
    message("find treejuce libraries")
    find_library(treejuce_main_lib ${treejuce_lib_name}
        HINTS
            ${TREEJUCE_SEARCH_PREFIX_LIB}
            ${TREEJUCE_SEARCH_PREFIX}/${CMAKE_INSTALL_LIBDIR}
            ${TREEJUCE_SEARCH_PREFIX}/lib
    )
    set_property(CACHE treejuce_main_lib
        PROPERTY
            ADVANCED 1
    )
    message(${treejuce_main_lib})
    set(TREEJUCE_LIBRARIES ${treejuce_dep_libs} ${treejuce_main_lib} CACHE STRING "" FORCE)
    message("for user: ${TREEJUCE_LIBRARIES}")
endif()

#
# finalize
#
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TREEJUCE
    REQUIRED_VARS
        treejuce_main_lib
        TREEJUCE_BIN_BUILDER
        TREEJUCE_INCLUDE_DIR
        TREEJUCE_LIBRARIES
)
