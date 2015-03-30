include(FindPackageMessage)
include(FindPackageHandleStandardArgs)

set(TREEJUCE_SEARCH_PREFIX "" CACHE PATH "additional path to search for treejuce")
set(TREEJUCE_SEARCH_PREFIX_INC "" CACHE PATH "additional path to search for treejuce headers")
set(TREEJUCE_SEARCH_PREFIX_LIB "" CACHE PATH "additional path to search for treejuce library")

# find header directory
find_path(TREEJUCE_INCLUDE_DIR treejuce/Config.h
    HINTS ${TREEJUCE_SEARCH_PREFIX_INC} ${TREEJUCE_SEARCH_PREFIX}/include
)

# find library
find_library(TREEJUCE_LIBRARIES treejuce
    HINTS ${TREEJUCE_SEARCH_PREFIX_LIB} ${TREEJUCE_SEARCH_PREFIX}/lib
)

# finalize
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TREEJUCE
    REQUIRED_VARS TREEJUCE_INCLUDE_DIR TREEJUCE_LIBRARIES
)
