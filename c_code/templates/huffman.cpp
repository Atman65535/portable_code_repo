#include "heap.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>

typedef struct HuffmanNode{
  uint8_t symbol;
  int weight;
  struct HuffmanNode* left;
  struct HuffmanNode* right;
  HuffmanNode(int weight): weight(weight), left(nullptr),
  right(nullptr){}
}HuffmanNode;

