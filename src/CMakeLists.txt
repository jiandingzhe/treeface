file(GLOB treeface_src_files "treeface/*.cpp")

add_library(treeface SHARED ${treeface_src_files})
