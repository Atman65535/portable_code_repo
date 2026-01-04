#pragma once
#include <vector>
#include <utility>

struct Edge {
  int to;
  int weight;
};

using Graph = std::vector<std::vector<Edge>>;
