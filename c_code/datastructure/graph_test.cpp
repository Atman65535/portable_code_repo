#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <iomanip>

using std::vector;
using std::cout;
using std::endl;

template <typename T>
class Dijkstra{
public:
  struct Edge {
    int to;
    T weight;
  };
  int n;
  vector<vector<Edge>> adj;

  Dijkstra(int n):n(n), adj(n){}
  // from u to v, weight is w
  void add_edge(int u, int v, T w) {
    adj[u].push_back({v, w});
  }

  vector<T> solve(int start) {
    vector<T> dist(n, std::nu
  }
};
