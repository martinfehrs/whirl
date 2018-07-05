cmake_minimum_required(VERSION 3.1)
project(LL1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(LL1 INTERFACE)
target_include_directories(LL1 INTERFACE Inc)
#target_sources(LL1 INTERFACE LL1.hpp)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ThirdParty/Catch2)

enable_testing(true)

add_executable(tests Tests/test.cpp)
target_link_libraries(tests LL1 Catch)

add_test(NAME static_look_ahead_buffer COMMAND tests static_look_ahead_buffer)
add_test(NAME is COMMAND tests [is])
add_test(NAME is_not COMMAND tests [is_not])
add_test(NAME is_one_of COMMAND tests [is_one_of])

