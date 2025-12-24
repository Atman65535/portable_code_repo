#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

#include "priority_queue.hpp"

using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;

typedef struct HuffmanNode {
    uint8_t symbol{};
    int weight;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
    uint8_t depth{};

    HuffmanNode() : symbol(0), weight(0), left(nullptr), right(nullptr), depth(0) {
    }

    explicit HuffmanNode(const int weight) : symbol(0), weight(weight), left(nullptr), right(nullptr),
                                             depth(0) {
    }
} HuffmanNode;

vector<HuffmanNode*> get_frequency(const string& file_name) {
    std::ifstream file_in(file_name, std::ios::binary);
    if (!file_in.is_open()) {
        std::cout << "file open failed!" << std::endl;
        exit(0);
    }
    vector<HuffmanNode*> weights(256);
    for (int i = 0; i < 256; i++) {
        weights[i]->symbol = static_cast<uint8_t>(i);
    }
    uint8_t byte;
    while (file_in.good()) {
        file_in.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t));
        weights[byte]->weight += 1;
    }
    return weights;
}

struct Cmp {
    bool operator()(HuffmanNode* a, HuffmanNode* b) const {
        return a->weight > b->weight;
    }
};

HuffmanNode* build_huff_tree(const vector<HuffmanNode*>& weights, int punish) {
    PriorityQueue<HuffmanNode*, Cmp> pq;
    vector<HuffmanNode*> parents;
    while (true) {
        while (!pq.empty())
            pq.pop();
        for (const auto& i : weights) {
            i->left = nullptr;
            i->right = nullptr;
            i->depth = 0;
            pq.push(i);
        }
        while (pq.size() > 1) {
            HuffmanNode* ret1 = pq.pop();
            HuffmanNode* ret2 = pq.pop();
            auto parent = new HuffmanNode();
            parent->depth = std::max(ret1->depth, ret2->depth) + 1;
            parent->left = ret1;
            parent->right = ret2;
            parent->weight = ret1->weight + ret2->weight + punish;
            parents.push_back(parent);
            pq.push(parent);
        }
        if (const auto root = pq.pop(); root->depth < 32) {
            return root;
        }
        for (const auto& i : parents)
            delete i;
        punish *= 2;
    }
}

typedef struct HuffCode {
    uint8_t symbol;
    uint32_t code;
    uint8_t len;

    bool operator <(const HuffCode& other) const {
        if (len < other.len)
            return true;
        if (len > other.len)
            return false;
        if (symbol < other.symbol)
            return true;
        return false;
    }
} HuffCode;

vector<HuffCode*> huffman_codes(256);

void get_huffman_len(vector<HuffCode*>& codes, const HuffmanNode* root, const int current_len) {
    if (root == nullptr) {
        std::cout << "empty root send in !" << std::endl;
        return;
    }
    if (root->left == nullptr && root->right == nullptr) {
        if (root->symbol > 255) {
            std::cout << "invalid root symbol !" << std::endl;
            exit(0);
        }
        codes[root->symbol]->symbol = root->symbol;
        codes[root->symbol]->len = current_len;
        return;
    }
    get_huffman_len(codes, root->left, current_len + 1);
    get_huffman_len(codes, root->right, current_len + 1);
}

class HuffmanCoder {
private:
    struct WriteBuf {
        uint64_t buffer;
        uint8_t buff_load;

        WriteBuf() : buffer(0), buff_load(0) {}

        void send_byte(ofstream& file_out) {
            uint8_t byte;
            byte = static_cast<uint8_t>(buffer >> 56);
            buffer <<= 8;
            file_out.write(reinterpret_cast<char*>(&byte), sizeof(uint8_t));
        }

        void write(ofstream& file_out, const uint32_t& code, const uint8_t len) {
            if (len > 32) {
                std::cout << "Invalid code ! check your program" << std::endl;
                exit(1);
            }
            buffer |= static_cast<uint64_t>(code) << (64 - len - buff_load);
            buff_load += len;

            while (buff_load >= 8) {
                send_byte(file_out);
                buff_load -= 8;
            }
        }

        void flush(ofstream& file_out) {
            while (buff_load >= 8) {
                send_byte(file_out);
                buff_load -= 8;
            }
            if (buff_load > 0) {
                send_byte(file_out);
                buff_load = 0;
            }
        }
    };

    vector<HuffCode*> encodes; // symbol sequence, visit via index. 'A' -> huff code of A
    vector<HuffCode*> decodes; // length sequence, visit via max_code and min_code and bias

    vector<uint32_t> max_code;
    vector<uint32_t> min_code;
    vector<uint8_t> min_code_index;
    vector<uint8_t> count;
    WriteBuf writer;

public:
    explicit HuffmanCoder(const vector<HuffCode*>& codes) :
        encodes(codes),
        decodes(codes),
        max_code(vector<uint32_t>(33, 0)),
        min_code(vector<uint32_t>(33, 0)),
        count(vector<uint8_t>(33, 0)),
        min_code_index(vector<uint8_t>(33, 0)){
        std::sort(decodes.begin(),
                  decodes.end(),
                  [](const HuffCode* a, const HuffCode* b) {
                      if (a->len != b->len) {
                          return a->len < b->len;
                      }
                      return a->symbol < b->symbol;
                  });
        uint8_t len = 0;
        uint32_t huffman_code = 0;
        for (const auto& code : decodes) {
            if (code->len == 0)
                continue; // jump off those zero len pnt
            if (len == 0) {
                code->code = huffman_code;
                len = code->len;
            }
            else {
                huffman_code += 1;
                if (code->len > len)
                    huffman_code <<= (code->len - len);
                code->code = huffman_code;
                len = code->len;
            }
        }
        for (int i = 0; i < 256; i++) {
            if (count[decodes[i]->len] == 0) {
                min_code[decodes[i]->len] = decodes[i]->code;
                min_code_index[decodes[i]->len] = i;
            }
            count[decodes[i]->len] ++;
            if (max_code[decodes[i]->len] < decodes[i]->code)
                max_code[decodes[i]->len] = decodes[i]->code;
        }
    }

    uint64_t huffman_encoder(const string& path_in, const string& path_out) {
        uint64_t code_counter = 0;
        ifstream file_in(path_in, std::ios::binary);
        ofstream file_out(path_out, std::ios::binary);
        uint8_t byte;
        while (file_in.good()) {
            file_in.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t));
            writer.write(file_out, encodes[byte]->code, encodes[byte]->len);
            code_counter += encodes[byte]->len;
        }
        file_in.close();
        file_out.close();
        return code_counter;
    }
    void huffman_decoder(const string& path_in, const string& path_out, uint64_t code_counter) const {
        ifstream file_in(path_in, std::ios::binary);
        ofstream file_out(path_out, std::ios::binary);
        uint8_t bit_cnt = 0;
        uint8_t byte = 0;
        uint32_t code = 0;
        uint8_t code_len = 0;
        while (code_counter) {
            if (bit_cnt == 0) { // read byte
                file_in.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t));
                bit_cnt += 8;
            }
            code = (code << 1) |((byte >> (bit_cnt - 1)) & 1);
            code_len ++;
            bit_cnt --;
            if (count[code_len]) {
                if (code <= max_code[code_len] && code >= min_code[code_len]) {
                    // must exist
                    uint8_t index = min_code_index[code_len] + code - min_code[code_len];
                    uint8_t symbol = decodes[index]->symbol;
                    file_out.write(reinterpret_cast<char*>(&symbol), sizeof(uint8_t));
                    code_counter -= code_len;
                    code_len = 0;
                    code = 0;
                }
            }
        }
        file_out.close();
        file_in.close();
    }
};


int main(int argc, char** argv) {
    if (argc > 2) {
        std::cout << "warning: argc > 2" << std::endl;
    }
    std::string file_name(argv[1]);
    auto weight = get_frequency(file_name);
    auto root = build_huff_tree(weight, 10);
    get_huffman_len(huffman_codes, root, 0);
}
