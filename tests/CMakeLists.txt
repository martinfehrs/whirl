add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE ${PROJECT_SOURCE_DIR}/third_party/catch2)

add_custom_target(valid_sequential_input)
add_custom_command(
    TARGET valid_sequential_input
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/sequential.inp
            ${CMAKE_CURRENT_BINARY_DIR}/sequential.inp
)

add_custom_target(invalid_sequential_input)
add_custom_command(
    TARGET invalid_sequential_input
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/sequential_invalid.inp
            ${CMAKE_CURRENT_BINARY_DIR}/sequential_invalid.inp
)

if(BUILD_TESTING)
    add_executable(tests tests.cpp)
else(BUILD_TESTING)
    add_executable(tests EXCLUDE_FROM_ALL tests.cpp)
endif(BUILD_TESTING)

target_link_libraries(tests PRIVATE whirl sequential_lib catch2)
add_dependencies(tests valid_sequential_input invalid_sequential_input)

add_test(NAME is         COMMAND tests [is]        )
add_test(NAME is-not     COMMAND tests [is-not]    )
add_test(NAME is-one-of  COMMAND tests [is-one-of] )
add_test(NAME is-none-of COMMAND tests [is-none-of])
add_test(NAME sequential COMMAND tests [sequential])
