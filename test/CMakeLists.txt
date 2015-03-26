find_package(SDL2 REQUIRED)
message(STATUS "SDL2:")
message(STATUS "  inc ${SDL2_INCLUDE_DIR}")
message(STATUS "  lib ${SDL2_LIBRARY}")

include_directories(${SDL2_INCLUDE_DIR})

add_executable(interact interact.cpp)
target_link_libraries(interact ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})

if(TREEFACE_OS STREQUAL "WINDOWS")
    set_target_properties(interact PROPERTIES
        WIN32_EXECUTABLE 1
    )
endif()
