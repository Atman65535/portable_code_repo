#include <ctime>
#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

using std::vector;

// Compare(father, son) == true
// invalid father and son relationship
//
template<typename T, typename Compare>
class PriorityQueue{
public:
  PriorityQueue() = default;

  void push(const T& value) {
    data.push_back(value);
    sift_up(data.size() - 1);
  }
  const T pop(void){
    assert(!data.empty());
    T retval = data[0];
    std::swap(data[0], data[data.size() - 1]);
    data.pop_back();
    sift_down(0);
    return retval;
  }
  bool empty(){
    return data.empty();
  }
  int size(){
    return data.size();
  }
private:
  vector<T> data;
  Compare cmp;
  const size_t parent(const size_t i) {
    return (i - 1) / 2;
  }
  const size_t left(const size_t i) {
    return 2 * i + 1;
  }
  const size_t right(const size_t i) {
    return 2 * i + 2;
  }

  void sift_down(size_t index) {
    while (true) {
      int left_son = left(index);
      int right_son = right(index);
      int max_node = index;
      if (left_son < data.size() && cmp(data[max_node], data[left_son]))
        max_node = left_son;
      if (right_son < data.size() && cmp(data[max_node], data[right_son]))
        max_node = right_son;
      if (max_node != index) {
        std::swap(data[max_node], data[index]);
        index = max_node;
      }
      else break;
    }
  }

  void sift_up(size_t index) {
    while(true) {
      int father = parent(index);
      if (father >= 0 && cmp(data[father], data[index])){
        std::swap(data[father], data[index]);
        index = father;
      }
      else break;
    }
  }
};

