configure_file(treeface/config.h.in treeface/config.h @ONLY)

file(GLOB treeface_src_files
    "treeface/*.h"
    "treeface/*.cpp"
    "treeface/*/*.h"
    "treeface/*/*.cpp"
)

add_library(treeface SHARED ${treeface_src_files})
