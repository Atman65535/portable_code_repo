#include "graph.hpp"
#include <functional>
#include <queue>
#include <vector>
#include <limits>

using std::vector;

/*
 * @brief dijkstra algorithm. One source minimal path.
 * @param graph, adj.
 *        source the source of graph
 */
vector<int> dijkstra(const Graph& graph, int source){
  const int INF = std::numeric_limits<int>::max();
  int n = graph.size();

  std::vector<int> dist(n, INF);
  dist[source] = 0;

  using P = std::pair<int, int>; // distance, node
  std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
  pq.push({0, source});

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();
    if (d > dist[u])
  }
}
