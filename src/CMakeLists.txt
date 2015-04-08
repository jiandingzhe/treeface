#
# configure file
#


#
# treeface
#
file(GLOB treeface_src_files
    "treeface/*.h"
    "treeface/*.cpp"
    "treeface/*/*.h"
    "treeface/*/*.cpp"
)

add_library(treeface STATIC ${treeface_src_files})




