add_custom_target(sequential_input)
add_custom_command(
        TARGET sequential_input
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/sequential_input.txt
                ${CMAKE_CURRENT_BINARY_DIR}/
)

add_executable(sequential sequential.cpp)
add_dependencies(sequential sequential_input)
target_link_libraries(sequential LLk)

