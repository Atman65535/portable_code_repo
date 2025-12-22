#pragma once  
#include <iostream>
#include <vector>
#include <string>

using std::cout, std::string, std::vector;

template <typename T> class Heap {
private: vector<T> &data;
  void build_heap(void){
    const int count = data.size();
    for (int i = father(count); i >=0; i--){
      sink(i);
    }
  }
  int father(int index){return (index-1)/2;}
  int left(int index){return 2 * index + 1;}
  int right(int index){return 2 * index + 2;}
  void drift_up(const int index){
    int current_index = index;
    while(true){
      const int parent = this->father(current_index);
      if (parent >= 0 && data[parent] < data[current_index]){
        std::swap(data[parent], data[current_index]);
        current_index = parent;
      }
      else{ break;
      }
    }
  }
  
  void sink(int index){
    int last_node = data.size() - 1;
    int current_index = index;
    while(true){
      int max_node = current_index;
      const int left_son = left(current_index);
      const int right_son = right(current_index);
      if(left_son <= last_node && data[left_son] > data[max_node])
        max_node = left_son;
      if(right_son <= last_node && data[right_son] > data[max_node])
        max_node = right_son;
      if (max_node == current_index){
        // already a heap
        break;
      }
      else {
        // change sequence.
        std::swap(data[current_index], data[max_node]);
        current_index = max_node;
      }
    }
  }
  
public:
  explicit Heap<T>(vector<T> &usr_data) : data(usr_data) {
    int loop = father(data.size()-1);
    for (; loop >= 0; loop--){
      sink(loop);
    }
  }
  void insert(const T input){
    data.push_back(input);
    drift_up(data.size()-1);
  }
  T pop(void){
    T head = data[0];
    data[0] = data[data.size() - 1];
    data.pop_back();
    sink(0);
    return head; 
  }
  bool empty(){
    return data.empty();
  }
  T look_top(){
    return data[0];
  }
};
