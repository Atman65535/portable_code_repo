cmake_minimum_required(VERSION 4.0)
project(c_code)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(templates)

add_executable(test1 test1.c)
add_executable(test2 test2.cpp)



add_executable(kmp kmp.cpp)
