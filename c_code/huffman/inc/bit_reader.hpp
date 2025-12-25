//
// Created by atman on 12/25/25.
//

#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../templates/priority_queue.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::shared_ptr;

/**
 * write bit. IO class.
 */
class BitReader {
private:
    ofstream file_in;
    uint64_t write_buf = 0;
    uint8_t bit_counter = 0;
    uint64_t total_write_bit = 0;

public:
    explicit BitReader(const string& in_path) : file_in(ofstream(out_path, std::ios::binary)) {
    }

    explicit BitWriter(ofstream file_out) : file_in(std::move(file_out)) {
    }

    ~BitWriter() {
        flush();
    }

    bool operator()(const uint32_t& code, const uint8_t& length) {
        if (length > 32 || length < 1)
            throw std::length_error("invalid code length.");
        while (file_in.good() && length + bit_counter > 64)
            write_byte();
        write_buf |= static_cast<uint64_t>(code) << (64 - length - bit_counter);
        bit_counter += length;
        // write when it can.
        while (file_in.good() && bit_counter >= 8)
            write_byte();
        return file_in.good();
    }

    bool flush() {
        while (file_in.good() && bit_counter) {
            write_byte();
        }
        return file_in.good();
    }

private:
    void write_byte() {
        uint8_t byte = static_cast<char>(write_buf >> 56);
        write_buf <<= 8;
        file_in.put(static_cast<char>(byte));
        if (bit_counter >= 8) {
            bit_counter -= 8;
            total_write_bit += 8;
        }
        else {
            total_write_bit += bit_counter;
            bit_counter = 0;
        }
    }
};
