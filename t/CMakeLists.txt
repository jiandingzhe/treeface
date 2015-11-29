add_library(TestFramework TestFramework.cpp)
target_include_directories(TestFramework PUBLIC ${TREECORE_INCLUDE_DIR})

add_executable(t_vec4 t_vec4.cpp)
target_use_treecore(t_vec4)
target_link_libraries(t_vec4
    treeface
    TestFramework
)

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

add_executable(t_quat t_quat.cpp)
target_use_treecore(t_quat)
target_link_libraries(t_quat
    treeface
    TestFramework
)

add_executable(t_vec3f t_vec3f.cpp)
target_link_libraries(t_vec3f
    TestFramework
)
target_use_treecore(t_vec3f)

add_executable(t_mat4 t_mat4.cpp)
target_use_treecore(t_mat4)
target_link_libraries(t_mat4
    treeface
    TestFramework
)

add_executable(t_object t_object.cpp)
target_use_treecore(t_object)
target_link_libraries(t_object
    treeface
    TestFramework
)


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

add_executable(t_package_manager_overwrite t_package_manager_overwrite.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_overwrite
    treeface
    TestFramework
)
target_use_treecore(t_package_manager_overwrite)

add_executable(t_package_manager_use_newer t_package_manager_use_newer.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_newer
    treeface
    TestFramework
)
target_use_treecore(t_package_manager_use_newer)

add_executable(t_package_manager_use_older t_package_manager_use_older.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_older
    treeface
    TestFramework
)
target_use_treecore(t_package_manager_use_older)

add_executable(t_image_manager t_image_manager.cpp)
target_link_libraries(t_image_manager
    treeface
    TestFramework
    ${FreeImage_LIBRARIES}
)
target_use_treecore(t_image_manager)

add_executable(t_vertex_template t_vertex_template.cpp)
target_link_libraries(t_vertex_template
    treeface
    TestFramework
)
target_use_treecore(t_vertex_template)

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

add_executable(t_property_validator t_property_validator.cpp)
target_link_libraries(t_property_validator
    treeface
    TestFramework
)
target_use_treecore(t_property_validator)

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

add_executable(t_widget_hierarchy t_widget_hierarchy.cpp)
target_link_libraries(t_widget_hierarchy
    treeface
    TestFramework
)
target_use_treecore(t_widget_hierarchy)
