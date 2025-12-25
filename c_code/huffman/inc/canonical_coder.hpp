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

#include "../../templates/priority_queue.hpp"

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
    CanonicalEncoderDecoder(const string& file_name, const bool is_raw) {
        if (is_raw) {
            // the file is the raw one, which haven't been compressed
            const vector<BuildNode> frequency_vec = init_from_raw(file_name);
            this->encode_list = canonical_freq2len(frequency_vec);
            this->decode_list = canonical_len2code(encode_list);
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

    void init_from_compress_list() {
    }

    void save_compress_list() {
    }

private:
    vector<shared_ptr<CanonCode>> encode_list;
    vector<shared_ptr<CanonCode>> decode_list;
};
