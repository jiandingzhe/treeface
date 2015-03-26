#
# configure
#

include(CheckTypeSize)
CHECK_TYPE_SIZE("void*" TREEFACE_PTR_SIZE)
CHECK_TYPE_SIZE("wchar_t" TREEFACE_WCHAR_SIZE)

message("wchar size: ${TREEFACE_WCHAR_SIZE}")

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

if(MSVC)
	# TODO
else()
	set_target_properties(treeface_zlib
		PROPERTIES
		COMPILE_FLAGS -fPIC
	)
endif()

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

if(TREEFACE_OS STREQUAL "LINUX")
    target_link_libraries(treeface pthread dl)
elseif(TREEFACE_OS STREQUAL "WINDOWS")
	target_link_libraries(treeface version winmm Shlwapi)
endif()

target_link_libraries(treeface
    treeface_zlib
)
