#pragma once
//
// Created by atman on 12/24/25.
//
/* canonical coder
 * canonical code, one type of huff code that doesn't rely on binary tree structure,
 * although it is actually a binary tree logically, but we don't organize it like
 * normal tree by pointer. We use bit-wise computation.
 *
 * This file have these functions:
 *  - frequency counter
 *  - get huff code length for each code
 *  - return encode and decode vectors.
 *
 * Ture encode and decode on file stage is implemented in huffman.hpp, then invoked in main.
 */
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::unique_ptr;

class CanonicalCoder {
private: // private structures
    using Freq = uint32_t;
    using Symbol = uint8_t;
    using CodeLen = uint8_t;

    struct BuildNode {
        Freq frequency;
        Symbol symbol;
        unique_ptr<BuildNode> left;
        unique_ptr<BuildNode> right;

        BuildNode() : frequency(0), symbol(0), left(nullptr), right(nullptr) {
        }
    };

    struct Cmp { // used for priority queue
        bool operator()(const BuildNode& father, const BuildNode& son) const {
            return father.frequency > son.frequency;
        }
    };

    struct CanonCode {
        Symbol symbol;
        CodeLen code_len;

        CanonCode():symbol(0), code_len(0) {}
    };

public:
    CanonicalCoder(const string& file_name, const bool is_raw) {
        if (is_raw) {
            // the file is the raw one, which haven't been compressed
            vector<BuildNode> frequency_vec = init_from_raw(file_name);

        }
        else {
        }
    }

private:
    static vector<BuildNode> init_from_raw(const string& file_name) {
        vector<BuildNode> frequency_counter(256); // invokes default constructor of BN
        // build symbol set
        for (int i = 0; i < 256; i++)
            frequency_counter[i].symbol = i;

        ifstream file_in(file_name, std::ios::binary);
        if (!file_in.is_open()) {
            std::cout << "file open failed !" << std::endl;
            exit(1);
        }
        uint8_t byte;
        // here reinterpret is safe. I know what I'm doing.
        while (file_in.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
            frequency_counter[byte].frequency += 1;
        }
        return frequency_counter;
    }

    /**
     * encoder list: visit code by index = symbol
     */
    static vector<CanonCode> get_encoder_list

    void init_from_compress_list() {
    }

    void save_compress_list() {
    }
};
