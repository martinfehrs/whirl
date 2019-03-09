add_custom_target(sequential_input)

add_library(sequential_lib INTERFACE)
target_include_directories(sequential_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sequential sequential.cpp)
target_link_libraries(sequential PUBLIC whirl sequential_lib)
