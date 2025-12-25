
add_executable(huffcode huffman_coder.cpp)
target_include_directories(huffcode PRIVATE ./inc)

target_link_libraries(huffcode PRIVATE nlohmann_json::nlohmann_json)