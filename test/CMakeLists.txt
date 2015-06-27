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

add_executable(scene_renderer scene_renderer.cpp)
target_link_libraries(scene_renderer
    treeface
    ${TREEJUCE_LIBRARIES}
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)

add_executable(scene_global_light scene_global_light.cpp)
target_link_libraries(scene_global_light
    treeface
    ${TREEJUCE_LIBRARIES}
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)

add_executable(sdl_event sdl_event.cpp)
target_link_libraries(sdl_event
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
)

if(TREEFACE_OS STREQUAL "WINDOWS")
    set_target_properties(interact interact_wrap texture geometry_and_material scene_renderer scene_global_light json_traverse freeimage_format sdl_event
        PROPERTIES
        WIN32_EXECUTABLE 0
    )
endif()
