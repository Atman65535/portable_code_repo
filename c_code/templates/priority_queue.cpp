#include "priority_queue.hpp"
#include <cstdio>
#include <utility>
#include <vector>

struct cmp {
  bool operator()(const int a , const int b) const {
    return a > b;
  }
}; 
