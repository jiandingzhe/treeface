add_library(TestFramework TestFramework.cpp)

add_executable(t_vec4 t_vec4.cpp)
target_link_libraries(t_vec4 treeface TestFramework)
