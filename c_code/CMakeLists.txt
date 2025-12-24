cmake_minimum_required(VERSION 3.20)
project(c_code)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(templates)
add_subdirectory(huffman)

add_executable(test1 test1.c)
add_executable(test2 test2.cpp)

add_executable(kmp kmp.cpp)
