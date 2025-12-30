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
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "priority_queue.hpp"
#include "bit_writer.hpp"
#include "bit_reader.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::shared_ptr;

class CanonicalEncoderDecoder {
private: // private structures
    using Freq = uint32_t;
    using Symbol = uint8_t;
    using CodeLen = uint8_t;

    struct BuildNode {
        Freq frequency;
        Symbol symbol;
        shared_ptr<BuildNode> left;
        shared_ptr<BuildNode> right;

        BuildNode() : frequency(0), symbol(0), left(nullptr), right(nullptr) {
        }
    };

    struct Cmp { // used for priority queue
        bool operator()(const shared_ptr<BuildNode>& father,
                        const shared_ptr<BuildNode>& son
        ) const {
            return father->frequency > son->frequency;
        }
    };

    struct CanonCode {
        Symbol symbol;
        CodeLen code_length;
        uint32_t code;

        CanonCode() : symbol(0), code_length(0), code(0) {
        }
    };

public:
    CanonicalEncoderDecoder(const string& path_in, const string& path_out, const bool is_compress) {
        this->path_in = path_in;
        this->path_out = path_out;
        ofstream file_out(path_out, std::ios::binary);
        ifstream file_in(path_in, std::ios::binary);
        if (is_compress) {
            // the file is the raw one, which haven't been compressed
            const vector<BuildNode> frequency_vec = init_from_raw(path_in);
            this->encode_list = canonical_freq2len(frequency_vec);
            this->decode_list = canonical_len2code(encode_list);
        }
        else {
            this->encode_list = init_from_compress_list(file_in);
            this->decode_list = canonical_len2code(encode_list);
        }
    }

    void encode() {
        ofstream file_out(path_out, std::ios::binary);
        file_out.seekp(head_offset);
        BitWriter writer(file_out); // send reference
        uint8_t byte;
        ifstream file_in(path_in, std::ios::binary);
        while (file_in.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t))) {
            compressed_symbols += 1;
            writer(encode_list[byte]->code, encode_list[byte]->code_length);
        }
        // safe bits
        writer(static_cast<uint32_t>(0), 32);
        writer(static_cast<uint32_t>(0), 32);
        writer.flush();
        save_compress_list(file_out);
    }

    void decode() {
        ifstream file_in(path_in, std::ios::binary);
        ofstream file_out(path_out, std::ios::binary);
        uint8_t min_len = decode_list[0]->code_length;
        uint8_t max_len = decode_list[decode_list.size() - 1]->code_length;
        vector<uint32_t> min_code(33, 0);
        vector<uint32_t> max_code(33, 0);
        vector<uint8_t> min_index(33, 0);
        for (int i = 0; i< decode_list.size(); i++) {
            uint8_t length = decode_list[i]->code_length;
            uint32_t code = decode_list[i]->code;
            if (i == 0) {
                min_code[length] = code;
                min_index[length] = i;
            }
            else if (length > decode_list[i - 1]->code_length) {
                min_code[length] = code;
                min_index[length] = i;
            }

            if (code > max_code[length])
                max_code[length] = code;
        }
        file_in.seekg(head_offset);
        BitReader reader(file_in);
        while (compressed_symbols) {
            for (int len = min_len; len <= max_len; len ++) {
                const uint32_t code = reader.peek_code_with_len(len);
                if (code >= min_code[len] && code <= max_code[len]) {
                    uint8_t symbol = decode_list[min_index[len] + code - min_code[len]]->symbol;
                    file_out.write(reinterpret_cast<char*>(&symbol), sizeof(uint8_t));
                    reader.remove_code(len);
                    compressed_symbols --;
                }
            }
        }
        file_in.close();
        file_out.close();
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
     * eg: encode['a'] = 010101...
     *     then b .... c.... d......
     * @return encode list. please use member encode_list to catch it.
     */
    static vector<shared_ptr<CanonCode>> canonical_freq2len(const vector<BuildNode>& frequency) {
        PriorityQueue<shared_ptr<BuildNode>, Cmp> huffman_heap;
        for (auto& ele : frequency) {
            // ignore 0 weight nodes
            if (ele.frequency > 0)
                huffman_heap.push(std::make_shared<BuildNode>(ele));
        }
        // build huff heap
        while (huffman_heap.size() > 1) {
            const auto ret1 = huffman_heap.pop();
            const auto ret2 = huffman_heap.pop();
            const auto parent = std::make_shared<BuildNode>();
            parent->frequency = ret1->frequency + ret2->frequency;
            parent->left = ret1;
            parent->right = ret2;
            huffman_heap.push(parent);
        }
        const auto root = huffman_heap.pop(); // last one

        // encode
        vector<shared_ptr<CanonCode>> encode_list(256);
        for (int i = 0; i < 256; i++) {
            encode_list[i] = std::make_shared<CanonCode>();
            encode_list[i]->symbol = i;
        }
        // get len
        get_canonical_code_len(encode_list, 0, root);
        // canonical time !
        return encode_list;
    }

    /**
     * get code length of each code.
     * DON'T attack it
     * @param word_list this is the vector encode_list
     * @param current_len the code len of this node. count UR edge
     * @param root root node absolutely.
     */
    static void get_canonical_code_len(vector<shared_ptr<CanonCode>>& word_list,
                                       const uint8_t& current_len,
                                       const shared_ptr<BuildNode>& root
    ) {
        if (!root) {
            cout << "why send into an empty tree? Adversarial ?" << endl;
        }
        // leaf
        if (root->left == nullptr && root->right == nullptr) {
            word_list[root->symbol]->code_length = (current_len > 0) ? current_len : 1;
            return;
        }
        get_canonical_code_len(word_list, current_len + 1, root->left);
        get_canonical_code_len(word_list, current_len + 1, root->right);
    }

    static vector<shared_ptr<CanonCode>> canonical_len2code(
        const vector<shared_ptr<CanonCode>>& list_with_len
    ) {
        if (list_with_len.empty())
            throw std::invalid_argument("canonical_len2code: are you kidding me?");
        vector<shared_ptr<CanonCode>> decodes(0);
        // we don't need zero code. Or we assign 0 to 0, which will be never used
        for (const auto& ele : list_with_len) {
            if (ele->code_length > 0) {
                decodes.push_back(ele);
            }
        }
        // sort it
        std::ranges::sort(decodes,
                          [](const shared_ptr<CanonCode>& a, const shared_ptr<CanonCode>& b) {
                              if (a->code_length != b->code_length)
                                  return a->code_length < b->code_length;
                              else
                                  return a->symbol < b->symbol;
                          });
        // assign code
        uint32_t current_code = 0;
        uint32_t current_length = decodes[0]->code_length;
        decodes[0]->code = 0;
        for (int i = 1; i < decodes.size(); i++) {
            current_code += 1;
            if (decodes[i]->code_length > current_length) {
                current_code <<= (decodes[i]->code_length - current_length);
                current_length = decodes[i]->code_length;
            }
            assert(current_length <= 32);
            decodes[i]->code = current_code;
        }
        return decodes;
    }

    vector<shared_ptr<CanonCode>> init_from_compress_list(ifstream& file_in) {
        std::vector<shared_ptr<CanonCode>> encodes(256);
        file_in.seekg(std::ios_base::beg);
        file_in.read(reinterpret_cast<char*>(&brand), sizeof(brand));
        file_in.read(reinterpret_cast<char*>(&compressed_symbols), sizeof(compressed_symbols));
        for (int i = 0; i < 256; i++) {
            encodes[i] = std::make_shared<CanonCode>();
            encodes[i]->symbol = i;
            file_in.read(reinterpret_cast<char*>(&encodes[i]->code_length),
                           sizeof(encodes[i]->code_length));
        }
        return encodes;
    }

    void save_compress_list(ofstream& file_out) {
        file_out.seekp(std::ios_base::beg);
        file_out.write(reinterpret_cast<char*>(&brand), sizeof(brand));
        file_out.write(reinterpret_cast<char*>(&compressed_symbols), sizeof(compressed_symbols));
        for (int i = 0; i < 256; i++) {
            file_out.write(reinterpret_cast<char*>(&encode_list[i]->code_length),
                           sizeof(encode_list[i]->code_length));
        }
    }

private:
    string path_in, path_out;
    vector<shared_ptr<CanonCode>> encode_list;
    vector<shared_ptr<CanonCode>> decode_list;
    uint64_t compressed_symbols = 0;
    uint32_t brand = 0x14451100;
    uint32_t head_offset = 4 + 8 + 256;
};
