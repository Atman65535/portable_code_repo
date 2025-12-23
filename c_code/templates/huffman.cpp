#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>
#include "priority_queue.hpp"

using std::vector;
using std::string;

typedef struct HuffmanNode{
  uint8_t symbol;
  int weight;
  struct HuffmanNode* left;
  struct HuffmanNode* right;
  HuffmanNode(): symbol(0), weight(0), left(nullptr), right(nullptr) {}
  HuffmanNode(int weight): weight(weight), left(nullptr),
  right(nullptr){}
}HuffmanNode;

vector<HuffmanNode*>  get_frequency(const string file_name) {
  std::ifstream file_in(file_name, std::ios::binary);
  if (!file_in.is_open()) {
    std::cout << "file open failed!" << std::endl;
    exit(0);
  }
  vector<HuffmanNode*> weights(256);
  for (int i = 0; i<256; i++) {
    weights[i]->symbol = static_cast<uint8_t>(i);
  }
  uint8_t byte;
  while (file_in.good()) {
    file_in.read(reinterpret_cast<char*>(&byte), sizeof(char));
    weights[byte]->weight += 1;
  }
  return weights;
}

struct Cmp {
  bool operator()(HuffmanNode* a, HuffmanNode* b) const {
    return a->weight > b->weight;
    }
};

HuffmanNode* build_huff_tree(vector<HuffmanNode*> weights) {
  PriorityQueue<HuffmanNode*, Cmp> pq;
  for (const auto& i : weights) {
    pq.push(i);
  }
  while(pq.size() > 1) {
    HuffmanNode* ret1 = pq.pop();
    HuffmanNode* ret2 = pq.pop();
    auto parent = new HuffmanNode(ret1->weight+ret2->weight);
    parent->left = ret1;
    parent->right = ret2;
    pq.push(parent);
  }
  return pq.pop();
}

vector<char*> get_huff_codeset() {
  return;
} 


int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "warning: argc > 2" << std::endl;
  }
  std::string file_name(argv[1]);
  auto weight = get_frequency(file_name);
  auto root = build_huff_tree(weight);
}
