#include "heap.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>

using std::ifstream;
using std::ofstream;
using std::vector;

// TODO : Add huff node heap. change the maximize method.

class HuffmanTree {
private:
  typedef struct huff_node {
    int weight;
    uint8_t word;
    huff_node() {
      weight = 0;
      word = 0;
    }
  } huff_node;

  class HuffHeap {
  private:
    vector<huff_node> &data;

    int left(const int index) { return (index * 2) + 1; }
    int right(const int index) { return (index * 2) + 2; }
    int father(const int index) { return (index - 1) / 2; }

    void sink_down(const int index) {
      int current_index = index;
      while (true) {
        int left_son = left(current_index);
        int right_son = right(current_index);
        int min_index = current_index;
        if (left_son < data.size() &&
            data[left_son].weight > data[min_index].weight)
          min_index = left_son;
        if (right_son < data.size() &&
            data[right_son].weight > data[min_index].weight)
          min_index = right_son;
        if (min_index == current_index)
          break;
        std::swap(data[min_index], data[current_index]);
        current_index = min_index;
      }
    }

    void drift_up(const int index) {
      int current_index = index;
      while (true) {
        int parent = father(current_index);
        if (parent >= 0 && data[parent].weight > data[current_index].weight) {
          std::swap(data[parent], data[current_index]);
          current_index = parent;
        }
        else
          break;
      }
    }

  public:
    HuffHeap(vector<huff_node> &usr_data) : data(usr_data) {
      for (int i = father(data.size() - 1); i >= 0; i--){
        sink_down(i);
      }
    }

    huff_node pop() {
      const huff_node ret = data[0];
      std::swap(data[0], data[data.size() - 1]);
      sink_down(0);
      return ret;
    }
    void push(const huff_node input){
      data.push_back(input);
      drift_up(data.size() - 1);
    }
  };
  
  std::unique_ptr<HuffHeap> huffman_tree_ptr = nullptr;
  string if_name;
  string of_name;

  vector<huff_node> count_appear_time(const string if_name) {
    ifstream file_in = ifstream(if_name, std::ios::binary);
    uint8_t single_word;
    vector<huff_node> appear_counter(256);
    for (int i = 0; i < 256; i++) {
      appear_counter[i].word = i;
      appear_counter[i].weight = 0;
    }
    while (file_in.good()) {
      file_in.read(reinterpret_cast<char *>(&single_word), sizeof(uint8_t));
      appear_counter[single_word].weight += 1;
    }
    return appear_counter;
  }

public:
  HuffmanTree(const string if_name, const string of_name):
  if_name(if_name), of_name(of_name) {
    vector<huff_node> appear_counter = count_appear_time(if_name);
    huffman_tree_ptr = std::make_unique<HuffHeap>(HuffHeap(appear_counter));
    
  }
};
