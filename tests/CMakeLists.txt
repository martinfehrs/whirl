add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE ${PROJECT_SOURCE_DIR}/third_party/catch2)


if(BUILD_TESTING)
    add_executable(tests tests.cpp)
else(BUILD_TESTING)
    add_executable(tests EXCLUDE_FROM_ALL tests.cpp)
endif(BUILD_TESTING)

target_link_libraries(tests LLk catch2)
add_test(NAME static_look_ahead_buffer COMMAND tests static_look_ahead_buffer)
add_test(NAME is COMMAND tests is)
add_test(NAME is-not COMMAND tests is-not)
add_test(NAME is-one-of COMMAND tests is-one-of)
