cmake_minimum_required(VERSION 3.9)
project(whirl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(whirl INTERFACE)
target_include_directories(whirl INTERFACE include)

#cmake option(BUILD_TESTING "" OFF)
include(CTest)
enable_testing(true)
add_subdirectory(examples)
add_subdirectory(tests)

install(FILES include/whirl.hpp DESTINATION include)
