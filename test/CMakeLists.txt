add_executable(interact interact.cpp)
target_link_libraries(interact ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})
target_use_treecore(interact)

add_executable(interact_wrap interact_wrap.cpp)
target_link_libraries(interact_wrap
    treeface
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
)
target_use_treecore(interact_wrap)

add_executable(texture texture.cpp)
target_link_libraries(texture
    treeface
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
target_use_treecore(texture)

add_executable(freeimage_format freeimage_format.cpp)
target_link_libraries(freeimage_format
    treeface
    ${FreeImage_LIBRARIES}
)
target_use_treecore(freeimage_format)

add_executable(json_traverse json_traverse.cpp)
target_use_treecore(json_traverse)

add_executable(geometry_and_material geometry_and_material.cpp)
target_link_libraries(geometry_and_material
    treeface
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
target_use_treecore(geometry_and_material)

add_executable(scene_renderer scene_renderer.cpp)
target_link_libraries(scene_renderer
    treeface
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
target_use_treecore(scene_renderer)

add_executable(scene_global_light scene_global_light.cpp)
target_link_libraries(scene_global_light
    treeface
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
target_use_treecore(scene_global_light)

add_executable(sdl_event sdl_event.cpp)
target_link_libraries(sdl_event
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
)

add_executable(sdl_keyboard_scancode sdl_keyboard_scancode.cpp)
target_link_libraries(sdl_keyboard_scancode
    ${SDL2_LIBRARY}
)

add_executable(sdl_text sdl_text.cpp)
target_link_libraries(sdl_text
    ${SDL2_LIBRARY}
)

add_executable(polygon_triangle polygon_triangle.cpp)
target_use_treecore(polygon_triangle)
target_link_libraries(polygon_triangle
    treeface
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
)

add_executable(polygon_monotone polygon_monotone.cpp)
target_use_treecore(polygon_monotone)
target_link_libraries(polygon_monotone
    treeface
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
)

add_executable(line_stroke line_stroke.cpp)
target_link_libraries(line_stroke treeface)
target_use_treecore(line_stroke)

add_executable(sdl_window_prop sdl_window_prop.cpp)
target_link_libraries(sdl_window_prop
    ${SDL2_LIBRARY}
)

add_executable(curve_edit curve_edit.cpp)
target_link_libraries(curve_edit
    treeface
    ${GLEW_LIBRARY}
    ${FreeImage_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${SDL2_LIBRARY}
)
target_use_treecore(curve_edit)

add_executable(fbodemo fbodemo.cpp)
target_link_libraries(fbodemo
    treeface
    ${GLEW_LIBRARY}
    ${FreeImage_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${SDL2_LIBRARY}
)
target_use_treecore(fbodemo)

if(TREEFACE_OS STREQUAL "WINDOWS")
    set_target_properties(
        interact
        interact_wrap
        texture
        geometry_and_material
        scene_renderer
        scene_global_light
        json_traverse
        freeimage_format
        sdl_event
        sdl_window_prop
        sdl_text
    PROPERTIES
        WIN32_EXECUTABLE 0
    )
endif()
