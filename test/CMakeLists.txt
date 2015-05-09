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
target_link_libraries(texture
    treeface
    ${FreeImage_LIBRARIES}
    ${TREEJUCE_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)

if(MSVC AND TREEFACE_OS STREQUAL "WINDOWS")
    set_target_properties(interact interact_wrap texture
        PROPERTIES
        WIN32_EXECUTABLE 0
    )
endif()

add_executable(freeimage_format freeimage_format.cpp)
target_link_libraries(freeimage_format
    treeface
    ${TREEJUCE_LIBRARIES}
    ${FreeImage_LIBRARIES}
)

add_executable(json_traverse json_traverse.cpp)
target_link_libraries(json_traverse
    ${TREEJUCE_LIBRARIES}
)

add_executable(geometry_and_material geometry_and_material.cpp)
target_link_libraries(geometry_and_material
    treeface
    ${TREEJUCE_LIBRARIES}
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
