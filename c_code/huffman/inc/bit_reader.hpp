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

#include "../../templates/priority_queue.hpp"

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
};
