cmake_minimum_required(VERSION 3.20)
project(c_code)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# my json is 3.11.3 (noble)
find_package(nlohmann_json 3.10.0 REQUIRED)

add_subdirectory(templates)
add_subdirectory(huffman)
add_subdirectory(datastructure)
add_subdirectory(datastructure/GraphAlgorithmsDemo)

add_executable(kmp kmp.cpp)
