//
// Created by atman on 12/25/25.
//

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "priority_queue.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::shared_ptr;
using std::string;
using std::vector;

/**
 * write bit. IO class.
 * this class can manage an ifstream, to read bits quickly
 */
class BitReader {
private:
<<<<<<< HEAD
  ifstream file_in;
  uint64_t read_buf = 0;
  uint8_t buf_bit_counter = 0;
  uint64_t total_unread_bit = 0; // this counter is for get one bit
  uint64_t file_bytes = 0;

public:
  BitReader(const string &in_name, uint64_t total_bits)
      : file_in(ifstream(in_name, std::ios::binary), total_unread_bit(total_bits), file_bytes(total_bits / 8 + 1)) {}
  BitReader(ifstream file_stream, uint64_t total_bits)
      : file_in(std::move(file_stream), total_unread_bit(total_bits), file_bytes(total_bits / 8 + 1)) {}
  
  uint8_t read_one_bit() {
    if (buf_bit_counter == 0)
      update_buf();
    uint8_t bit = static_cast<uint8_t>(read_buf >> 63);
    read_buf <<= 1;
    buf_bit_counter -= 1;
    total_unread_bit -= 1;
    return bit;
  }
  const bool read_over() {
    return total_unread_bit == 0;
  }
private:
    void update_buf() {// update 32 bits once
      if (file_bytes >= 8) {
        file_in.read(reinterpret_cast<char*>(&read_buf), sizeof(uint32_t));
        buf_bit_counter += 64;
        file_bytes -= 8;
      }
      else {
        file_in.read(reinterpret_cast<char*>(&read_buf), file_bytes);
        buf_bit_counter += file_bytes * 8;
        file_bytes = 0;
      }
    }    
=======
    ifstream& file_in;
    uint64_t read_buf = 0;
    uint8_t bit_counter = 0;

public:
    explicit BitReader(ifstream& in_file) : file_in(in_file) {
    }

    /**
     * usage : for i in range(min len, max len+1):
     *              peek; remove_code if matched, else continue.
     * @param len len of code. from shortest to longest.
     * @return the code of this length.
     */
    uint32_t peek_code_with_len(const uint8_t len) {
        if (bit_counter < 32)
            read_file();
        const uint32_t ret = read_buf >> (64 - len);
        return ret;
    }
    void remove_code(const uint8_t len) {
        read_buf <<= len;
        bit_counter -= len;
    }
private:
    /**
     * this function read 4 byte at once.
     */
    void read_file() {
        assert(file_in.good());
        // we will pad some 0 at the tail of huffman code
        if (bit_counter < 32) {
            uint32_t temp_buf = 0;
            uint8_t byte;
            for (int i = 0; i< 4; i++) {
                if (file_in.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t)))
                    temp_buf = (temp_buf << 8) | static_cast<uint32_t>(byte);
                else // dead line.
                    temp_buf = temp_buf << 8;
            }
            read_buf |= static_cast<uint64_t>(temp_buf) << (64 - 32 - bit_counter);
            bit_counter += 32;
        }
    }
>>>>>>> c4abddb (a)
};
