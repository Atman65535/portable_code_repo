//
// Created by atman on 12/25/25.
//#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>

class BitReader {
private:
    std::ifstream file_in;
    uint64_t read_buf = 0;    // 64位仓库
    uint8_t bit_counter = 0;   // 仓库里现在有多少位可用
    uint64_t total_bits = 0;   // 整个文件的有效位数（从文件头读取）
    uint64_t processed_bits = 0; // 已经解了多少位

public:
    // 构造函数：打开文件并读取你之前存的总位数
    explicit BitReader(const std::string& path) : file_in(path, std::ios::binary) {
        if (!file_in) throw std::runtime_error("Cannot open file");

        // 假设你的文件头前8个字节存的是 total_bits
        file_in.read(reinterpret_cast<char*>(&total_bits), sizeof(uint64_t));
    }

    // 核心函数：读取指定长度的位
    uint32_t read_bits(uint8_t length) {
        if (length > 32) throw std::length_error("Read length too long");
        if (processed_bits + length > total_bits) {
            // 这里的处理很重要：如果剩下的不够读了，说明文件损坏或逻辑有误
            length = static_cast<uint8_t>(total_bits - processed_bits);
        }

        // 1. 确保仓库里有足够的位供读取
        while (bit_counter < length && file_in.good()) {
            fill_buffer();
        }

        // 2. 从 64 位仓库的高位切下所需长度
        // 假设数据是靠左对齐存入的
        uint32_t result = static_cast<uint32_t>(read_buf >> (64 - length));

        // 3. 仓库左移，踢掉已读数据，并更新计数
        read_buf <<= length;
        bit_counter -= length;
        processed_bits += length;

        return result;
    }

    bool has_more() const {
        return processed_bits < total_bits;
    }

private:
    void fill_buffer() {
        // 从文件读一个字节
        char byte;
        if (file_in.get(byte)) {
            // 把这 8 位塞进仓库右侧（空余位置）
            // 比如仓库里已有 10 位，新字节要放在 64-10-8 = 46 位偏移处
            read_buf |= (static_cast<uint64_t>(static_cast<uint8_t>(byte)) << (64 - 8 - bit_counter));
            bit_counter += 8;
        }
    }
};

// 伪代码逻辑
uint32_t current_code = 0;
uint8_t current_len = 0;

while (reader.has_more()) {
    // 每次读 1 位
    current_code = (current_code << 1) | reader.read_bits(1);
    current_len++;

    // 去你之前存的规范编码表里查：
    // 是否存在一个长度为 current_len 且值为 current_code 的编码？
    if (match_found) {
        output_file.put(symbol);
        current_code = 0; // 重置，准备解下一个字符
        current_len = 0;
    }
}