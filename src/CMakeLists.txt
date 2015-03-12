file(GLOB treeface_src_files "treeface/*.cpp")

add_library(treeface SHARED ${treeface_src_files})
set_target_properties(treeface PROPERTIES
    COMPILE_FLAGS -std=c++11
)
