cmake_minimum_required(VERSION 3.1)
project(LLk)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(LLk INTERFACE)
target_include_directories(LLk INTERFACE include)

#cmake option(BUILD_TESTING "" OFF)
include(CTest)
enable_testing(true)
add_subdirectory(tests)

add_subdirectory(examples)

install(FILES Inc/LLk.hpp DESTINATION include)

