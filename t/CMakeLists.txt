add_library(TestFramework TestFramework.cpp)

add_executable(t_vec4 t_vec4.cpp)
target_link_libraries(t_vec4 treeface TestFramework)

add_executable(t_quat t_quat.cpp)
target_link_libraries(t_quat treeface TestFramework)

add_executable(t_mat4 t_mat4.cpp)
target_link_libraries(t_mat4 treeface TestFramework)

add_executable(t_object t_object.cpp)
target_link_libraries(t_object treeface TestFramework)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp
    COMMAND ${TREEJUCE_BIN_BUILDER} ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR} resources
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(t_package_manager_keep_existing t_package_manager_keep_existing.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_keep_existing ${TREEJUCE_LIBRARIES} treeface TestFramework)

add_executable(t_package_manager_overwrite t_package_manager_overwrite.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_overwrite ${TREEJUCE_LIBRARIES} treeface TestFramework)

add_executable(t_package_manager_use_newer t_package_manager_use_newer.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_newer ${TREEJUCE_LIBRARIES} treeface TestFramework)

add_executable(t_package_manager_use_older t_package_manager_use_older.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
target_link_libraries(t_package_manager_use_older ${TREEJUCE_LIBRARIES} treeface TestFramework)
