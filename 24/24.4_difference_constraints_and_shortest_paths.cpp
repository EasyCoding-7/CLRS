#include <cassert>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <tuple>
#include <concepts>
#include <limits>

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <std::size_t n, arithmetic T = double, bool undirected = false>
class Graph {
    std::vector<std::list<std::pair<std::size_t, T>>> adj;
    std::unordered_map<std::size_t, typename std::list<std::pair<std::size_t, T>>::iterator> edges;

    struct PathInfo {
        T d = T{0};
        std::size_t parent = -1;
    };

public:
    Graph() : adj(n), pinfos(n) {
    }

    std::vector<PathInfo> pinfos;

    void addEdge(std::size_t src, std::size_t dst, T weight) {
        assert(src < n && dst < n);
        if (!edges.contains(src * n + dst)) {
            adj[src].emplace_front(dst, weight);
            edges[src * n + dst] = adj[src].begin();
        }
        if (undirected) {
            if (!edges.contains(dst * n + src)) {
                adj[dst].emplace_front(src, weight);
                edges[dst * n + src] = adj[dst].begin();
            }
        }
    }

    void removeEdge(std::size_t src, std::size_t dst) {
        assert(src < n && dst < n);
        adj[src].erase(edges[src * n + dst]);
        edges.erase(src * n + dst);
        if (undirected) {
            adj[dst].erase(edges[dst * n + src]);
            edges.erase(dst * n + src);
        }
    }

    [[nodiscard]] T getWeight(std::size_t src, std::size_t dst) const {
        assert(src < n && dst < n);
        if (!edges.contains(src * n + dst)) {
            return T {0};
        } else {
            return edges.at(src * n + dst)->second;
        }
    }

    using Edge = std::tuple<std::size_t, std::size_t, T>;

    void InitializeSingleSource(std::size_t s) {
        pinfos.resize(0);
        pinfos.resize(n);
        pinfos[s].d = T {0};
    }

    void Relax(std::size_t u, std::size_t v) {
        T w = getWeight(u, v);
        if (pinfos[v].d > pinfos[u].d + w) {
            pinfos[v].d = pinfos[u].d + w;
            pinfos[v].parent = u;
        }
    }

    bool BellmanFord(std::size_t s) {
        InitializeSingleSource(s);
        for (std::size_t i = 0; i < n - 1; i++) {
            for (std::size_t u = 0; u < n; u++) {
                for (const auto& [v, w] : adj[u]) {
                    Relax(u, v);
                }
            }
        }
        for (std::size_t u = 0; u < n; u++) {
            for (const auto& [v, w] : adj[u]) {
                if (pinfos[v].d > pinfos[u].d + w) {
                    return false;
                }
            }
        }
        for (std::size_t u = 0; u < n; u++) {
            std::cout << pinfos[u].d << ' ';
        }
        std::cout << '\n';
        return true;
    }

};

int main() {
    Graph<6> g;
    g.addEdge(1, 0, 1);
    g.addEdge(3, 0, -4);
    g.addEdge(2, 1, 2);
    g.addEdge(4, 1, 7);
    g.addEdge(5, 1, 5);
    g.addEdge(5, 2, 10);
    g.addEdge(1, 3, 2);
    g.addEdge(0, 4, -1);
    g.addEdge(3, 4, 3);
    g.addEdge(2, 5, -8);
    assert(g.BellmanFord(0));

}