configure_file(treeface/config.h.in treeface/config.h @ONLY)

include_directories(zlib)

#
# zlib
#
file(GLOB zlib_src_files
    "zlib/*.h"
    "zlib/*.c"
)

# add to a separate target
# otherwise it won't use C compiler
add_library(treeface_zlib STATIC ${zlib_src_files})

set_target_properties(treeface_zlib
    PROPERTIES
    COMPILE_FLAGS -fPIC
)

#
# treeface
#
file(GLOB treeface_src_files
    "treeface/*.h"
    "treeface/*.cpp"
    "treeface/*/*.h"
    "treeface/*/*.cpp"
    "treejuce/*.h"
    "treejuce/*.cpp"
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    file(GLOB treeface_platform_src_files
        "treejuce/native/linux_*.h"
        "treejuce/native/linux_*.cpp"
        "treejuce/native/posix_*.h"
        "treejuce/native/posix_*.cpp"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    file(GLOB treeface_platform_src_files
        "treejuce/native/win32_*.h"
        "treejuce/native/win32_*.cpp"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    file(GLOB treeface_platform_src_files
        "treejuce/native/mac_*.mm"
        "treejuce/native/osx_*.h"
        "treejuce/native/osx_*.cpp"
        "treejuce/native/posix_*.h"
        "treejuce/native/posix_*.cpp"
    )
else()
    message(SEND_ERROR "unsupported target system: ${CMAKE_SYSTEM_NAME}")
endif()

add_library(treeface SHARED ${treeface_src_files} ${treeface_platform_src_files})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(treeface pthread dl)
endif()

target_link_libraries(treeface
    treeface_zlib
)
