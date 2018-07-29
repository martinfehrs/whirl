add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE ${PROJECT_SOURCE_DIR}/third_party/catch2)


if(BUILD_TESTING)
    add_executable(tests tests.cpp)
else(BUILD_TESTING)
    add_executable(tests EXCLUDE_FROM_ALL tests.cpp)
endif(BUILD_TESTING)

target_link_libraries(tests LLk catch2)
add_test(NAME is COMMAND tests [is])
add_test(NAME is-not COMMAND tests [is-not])
add_test(NAME is-one-of COMMAND tests [is-one-of])
add_test(NAME is-none-of COMMAND tests [is-none-of])

add_test(NAME sequential COMMAND sequential WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
set_tests_properties (sequential PROPERTIES PASS_REGULAR_EXPRESSION "^-3[\r\n\t ]*-3[\r\n\t ]*-2[\r\n\t ]*-1[\r\n\t ]*0[\r\n\t ]*-1[\r\n\t ]*0[\r\n\t ]*1[\r\n\t ]*1[\r\n\t ]*2[\r\n\t ]*2[\r\n\t ]*2[\r\n\t ]*4[\r\n\t ]*5[\r\n\t ]*8[\r\n\t ]*")


