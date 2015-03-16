add_library(TestFramework TestFramework.cpp)

add_executable(t_vec4 t_vec4.cpp)
target_link_libraries(t_vec4 treeface TestFramework)

add_executable(t_simd t_simd.cpp)
target_link_libraries(t_simd treeface TestFramework)

add_executable(t_atomic t_atomic.cpp)
target_link_libraries(t_atomic treeface TestFramework)

add_executable(t_object t_object.cpp)
target_link_libraries(t_object treeface TestFramework)
