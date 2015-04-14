include(FindPackageMessage)
include(FindPackageHandleStandardArgs)
include(GNUInstallDirs)

set(FreeImage_SEARCH_PREFIX      "" CACHE PATH   "additional path to search for SDL2")
set(FreeImage_SEARCH_PREFIX_INC  "" CACHE PATH   "additional path to search for SDL2 header SDL.h")
set(FreeImage_SEARCH_PREFIX_LIB  "" CACHE PATH   "additional path to search for SDL2 library")
set(FreeImage_SEARCH_32BIT_64BIT 64 CACHE STRING "search for 32bit or 64bit library")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(freeimage_lib_name FreeImage)
    if(FreeImage_SEARCH_32BIT_64BIT EQUAL 32)
        set(freeimage_inc_hint
            ${FreeImage_SEARCH_PREFIX}/Dist/x32
            ${FreeImage_SEARCH_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
            ${FreeImage_SEARCH_PREFIX}/include
        )
        set(freeimage_lib_hint
            ${FreeImage_SEARCH_PREFIX}/Dist/x32
            ${FreeImage_SEARCH_PREFIX}/${CMAKE_INSTALL_LIBDIR}
            ${FreeImage_SEARCH_PREFIX}/lib
        )
    elseif(FreeImage_SEARCH_32BIT_64BIT 64)
        set(freeimage_inc_hint
            ${FreeImage_SEARCH_PREFIX}/Dist/x64
            ${FreeImage_SEARCH_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
            ${FreeImage_SEARCH_PREFIX}/include
        )
        set(freeimage_lib_hint
            ${FreeImage_SEARCH_PREFIX}/Dist/x64
            ${FreeImage_SEARCH_PREFIX}/${CMAKE_INSTALL_LIBDIR}
            ${FreeImage_SEARCH_PREFIX}/lib64
        )
    else()
        message(FATAL_ERROR "invalid bit size: ${FreeImage_SEARCH_32BIT_64BIT}, must be 32 or 64")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(freeimage_lib_name freeimage)
    set(freeimage_inc_hint
        ${FreeImage_SEARCH_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
        ${FreeImage_SEARCH_PREFIX}/include
    )
    set(freeimage_lib_hint
        ${FreeImage_SEARCH_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        ${FreeImage_SEARCH_PREFIX}/lib
    )
else()
    message(FATAL_ERROR "unsupported system \"${CMAKE_SYSTEM_NAME}\"")
endif()

#
# do find
#
find_path(FreeImage_INCLUDE_DIR FreeImage.h
    HINTS ${FreeImage_SEARCH_PREFIX_INC} ${freeimage_inc_hint})

find_library(FreeImage_LIBRARIES ${freeimage_lib_name}
    HINTS ${FreeImage_SEARCH_PREFIX_LIB} ${freeimage_lib_hint})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(FreeImage
    REQUIRED_VARS FreeImage_INCLUDE_DIR FreeImage_LIBRARIES
)
