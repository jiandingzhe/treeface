add_library(TestFramework TestFramework.cpp)

add_executable(t_vec4 t_vec4.cpp)
target_link_libraries(t_vec4
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_sdl2_leak t_sdl2_leak.cpp)
target_link_libraries(t_sdl2_leak
    treeface
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_quat t_quat.cpp)
target_link_libraries(t_quat
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_mat4 t_mat4.cpp)
target_link_libraries(t_mat4
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_object t_object.cpp)
target_link_libraries(t_object
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp
    COMMAND ${TREEJUCE_BIN_BUILDER} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR} resources
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(t_package_manager_keep_existing t_package_manager_keep_existing.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_keep_existing
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_package_manager_overwrite t_package_manager_overwrite.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_overwrite
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_package_manager_use_newer t_package_manager_use_newer.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_newer
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_package_manager_use_older t_package_manager_use_older.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_older
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_image_manager t_image_manager.cpp)
target_link_libraries(t_image_manager
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
    ${FreeImage_LIBRARIES}
)

add_executable(t_vertex_template t_vertex_template.cpp)
target_link_libraries(t_vertex_template
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_material t_material.cpp)
target_link_libraries(t_material
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)

add_executable(t_property_validator t_property_validator.cpp)
target_link_libraries(t_property_validator
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)

add_executable(t_geometry t_geometry.cpp)
target_link_libraries(t_geometry
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)

add_executable(t_node_manager t_node_manager.cpp)
target_link_libraries(t_node_manager
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${FreeImage_LIBRARIES}
)

add_executable(t_ned_object t_ned_object.cpp)
target_link_libraries(t_ned_object
    treeface
    TestFramework
    ${TREEJUCE_LIBRARIES}
)
