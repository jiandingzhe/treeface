find_package(SDL2 REQUIRED)
message(STATUS "SDL2:")
message(STATUS "  inc ${SDL2_INCLUDE_DIR}")
message(STATUS "  lib ${SDL2_LIBRARY}")

include_directories(${SDL2_INCLUDE_DIR})

add_executable(interact interact.cpp)
target_link_libraries(interact ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})

add_executable(interact_wrap interact_wrap.cpp)
target_link_libraries(interact_wrap
    treeface
    ${TREEJUCE_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
)

add_executable(texture texture.cpp)
target_link_libraries(texture treeface ${TREEJUCE_LIBRARIES} ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

if(MSVC AND TREEFACE_OS STREQUAL "WINDOWS")
    set_target_properties(interact interact_wrap texture
        PROPERTIES
        WIN32_EXECUTABLE 0
    )
endif()
