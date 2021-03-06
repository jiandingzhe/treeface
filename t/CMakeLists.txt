add_library(TestFramework TestFramework.cpp)
target_include_directories(TestFramework PUBLIC ${TREECORE_INCLUDE_DIR})

add_executable(t_steaking_array t_steaking_array.cpp)
target_link_libraries(t_steaking_array TestFramework)
target_use_treecore(t_steaking_array)
add_test(NAME t_steaking_array COMMAND t_steaking_array)

add_executable(t_vec4 t_vec4.cpp)
target_use_treecore(t_vec4)
target_link_libraries(t_vec4
    treeface
    TestFramework
)
add_test(NAME t_vec4 COMMAND t_vec4)

add_executable(t_simd_xor_flip t_simd_xor_flip.cpp)
target_link_libraries(t_simd_xor_flip
    TestFramework
)
target_use_treecore(t_simd_xor_flip)
add_test(NAME t_simd_xor_flip COMMAND t_simd_xor_flip)

add_executable(t_sdl2_leak t_sdl2_leak.cpp)
target_use_treecore(t_sdl2_leak)
target_link_libraries(t_sdl2_leak
    treeface
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    TestFramework
)
add_test(NAME t_sdl2_leak COMMAND t_sdl2_leak)

add_executable(t_quat t_quat.cpp)
target_use_treecore(t_quat)
target_link_libraries(t_quat
    treeface
    TestFramework
)
add_test(NAME t_quat COMMAND t_quat)


add_executable(t_vec2f t_vec2f.cpp)
target_link_libraries(t_vec2f TestFramework)
target_use_treecore(t_vec2f)
add_test(NAME t_vec2f COMMAND t_vec2f)

add_executable(t_vec3f t_vec3f.cpp)
target_link_libraries(t_vec3f
    TestFramework
)
target_use_treecore(t_vec3f)
add_test(NAME t_vec3f COMMAND t_vec3f)

add_executable(t_mat2f t_mat2f.cpp)
target_use_treecore(t_mat2f)
target_link_libraries(t_mat2f
    treeface
    TestFramework
)
add_test(NAME t_mat2f COMMAND t_mat2f)

add_executable(t_mat3 t_mat3.cpp)
target_use_treecore(t_mat3)
target_link_libraries(t_mat3
    treeface
    TestFramework
)
add_test(NAME t_mat3 COMMAND t_mat3)

add_executable(t_mat4 t_mat4.cpp)
target_use_treecore(t_mat4)
target_link_libraries(t_mat4
    treeface
    TestFramework
)
add_test(NAME t_mat4 COMMAND t_mat4)

add_executable(t_mat4_quat_rot t_mat4_quat_rot.cpp)
target_use_treecore(t_mat4_quat_rot)
target_link_libraries(t_mat4_quat_rot
    treeface
    TestFramework
)
add_test(NAME t_mat4_quat_rot COMMAND t_mat4_quat_rot)

add_executable(t_object t_object.cpp)
target_use_treecore(t_object)
target_link_libraries(t_object
    treeface
    TestFramework
)
add_test(NAME t_object COMMAND t_object)

add_executable(t_line_cross t_line_cross.cpp)
target_use_treecore(t_line_cross)
target_link_libraries(t_line_cross
    treeface
    TestFramework
)
add_test(NAME t_line_cross COMMAND t_line_cross)

#add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp
#    COMMAND ${TREECORE_BIN_BUILDER} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR} resources
#)
treecore_wrap_resource(resources resources)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(t_package_manager_keep_existing t_package_manager_keep_existing.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_keep_existing
    treeface
    TestFramework
)
target_use_treecore(t_package_manager_keep_existing)
add_test(NAME t_package_manager_keep_existing COMMAND t_package_manager_keep_existing)

add_executable(t_package_manager_overwrite t_package_manager_overwrite.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_overwrite
    treeface
    TestFramework
)
target_use_treecore(t_package_manager_overwrite)
add_test(NAME t_package_manager_overwrite COMMAND t_package_manager_overwrite)

add_executable(t_package_manager_use_newer t_package_manager_use_newer.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_newer
    treeface
    TestFramework
)
target_use_treecore(t_package_manager_use_newer)
add_test(NAME t_package_manager_use_newer COMMAND t_package_manager_use_newer)

add_executable(t_package_manager_use_older t_package_manager_use_older.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_older
    treeface
    TestFramework
)
target_use_treecore(t_package_manager_use_older)
add_test(NAME t_package_manager_use_older COMMAND t_package_manager_use_older)

add_executable(t_image_manager t_image_manager.cpp)
target_link_libraries(t_image_manager
    treeface
    TestFramework
    ${FreeImage_LIBRARIES}
)
target_use_treecore(t_image_manager)
add_test(NAME t_image_manager COMMAND t_image_manager)

add_executable(t_vertex_template t_vertex_template.cpp)
target_link_libraries(t_vertex_template
    treeface
    TestFramework
)
target_use_treecore(t_vertex_template)
add_test(NAME t_vertex_template COMMAND t_vertex_template)

add_executable(t_material t_material.cpp)
target_link_libraries(t_material
    treeface
    TestFramework
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
target_use_treecore(t_material)
add_test(NAME t_material COMMAND t_material)

add_executable(t_property_validator t_property_validator.cpp)
target_link_libraries(t_property_validator
    treeface
    TestFramework
)
target_use_treecore(t_property_validator)
add_test(NAME t_property_validator COMMAND t_property_validator)

add_executable(t_geometry t_geometry.cpp)
target_link_libraries(t_geometry
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
target_use_treecore(t_geometry)
add_test(NAME t_geometry COMMAND t_geometry)

add_executable(t_geometry_load_align t_geometry_load_align.cpp)
target_link_libraries(t_geometry_load_align
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
target_use_treecore(t_geometry_load_align)
add_test(NAME t_geometry_load_align COMMAND t_geometry_load_align)

add_executable(t_node_manager t_node_manager.cpp)
target_link_libraries(t_node_manager
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${FreeImage_LIBRARIES}
)
target_use_treecore(t_node_manager)
add_test(NAME t_node_manager COMMAND t_node_manager)

add_executable(t_scene t_scene.cpp)
target_link_libraries(t_scene
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${FreeImage_LIBRARIES}
)
target_use_treecore(t_scene)
add_test(NAME t_scene COMMAND t_scene)

add_executable(t_widget_hierarchy t_widget_hierarchy.cpp)
target_link_libraries(t_widget_hierarchy
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${GLEW_LIBRARY}
)
target_use_treecore(t_widget_hierarchy)
add_test(NAME t_widget_hierarchy COMMAND t_widget_hierarchy)

add_executable(t_geom_visual_obj_chain t_geom_visual_obj_chain.cpp)
target_link_libraries(t_geom_visual_obj_chain
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${FreeImage_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${GLEW_LIBRARY}
)
target_use_treecore(t_geom_visual_obj_chain)
add_test(NAME t_geom_visual_obj_chain COMMAND t_geom_visual_obj_chain)

add_executable(t_visual_obj_uniform_hier t_visual_obj_uniform_hier.cpp)
target_link_libraries(t_visual_obj_uniform_hier
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${FreeImage_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${GLEW_LIBRARY}
)
target_use_treecore(t_visual_obj_uniform_hier)
add_test(NAME t_visual_obj_uniform_hier COMMAND t_visual_obj_uniform_hier)

add_executable(t_material_resource t_material_resource.cpp)
target_link_libraries(t_material_resource
    treeface
    TestFramework
    ${SDL2_LIBRARY}
    ${FreeImage_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${GLEW_LIBRARY}
)
target_use_treecore(t_material_resource)
add_test(NAME t_material_resource COMMAND t_material_resource)
