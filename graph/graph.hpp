#pragma once
#include <algorithm>
#include <assert.h>
#include <queue>
#include <stack>
#include <tuple>
#include <vector>

#include "../data_structure/union_find.hpp"
#include "../math/matrix.hpp"
#include "edge.hpp"

/**
 * @brief グラフを管理するクラス。
 * @tparam WEIGHT int なら重みなし、そうでないなら重みつきグラフ
 * @tparam is_directed 有向グラフかとうか
 */
template <typename WEIGHT, bool is_directed> class graph {
  private:
    int N;
    std::vector<std::vector<edge<WEIGHT>>> G;
    std::vector<edge<WEIGHT>> E;
    union_find uf;
    WEIGHT W = 0;

    mutable std::vector<bool> visited; // dfs / bfs のための領域

  public:
    graph() : N(0){};
    graph(int n) : N(n), G(n), uf(n), visited(n){};

    /**
     * @brief ノードの数をn個まで増やす
     * @param n サイズ
     * @attention 今のノード数より小さい数を渡したとき、変化なし
     */
    void expand(int n) {
        if (n <= N)
            return;
        N = n;
        G.resize(n);
        uf.expand(n);
        visited.resize(n);
    }

    /**
     * @return ノードの数
     */
    int count_nodes() const { return N; }

    /**
     * @return 辺の数
     */
    int count_edges() const { return E.size(); }

    /**
     * @param n ノード番号
     * @return ノード n からの隣接頂点のリストの const 参照
     */
    const std::vector<edge<WEIGHT>> &operator[](int n) const { return G[n]; }

    /**
     * @return グラフ全体の辺のリストの const 参照
     */
    const std::vector<edge<WEIGHT>> &edges() const { return E; }

    /**
     * @param x ノード番号
     * @param y ノード番号
     * @return x, y が連結かどうか
     */
    bool are_connected(int x, int y) const { return uf.same(x, y); }

    /**
     * @return 連結成分の数
     */
    int count_connected_components() const { return uf.count_groups(); }

    /**
     * @return 連結成分のリストのリスト
     */
    std::vector<std::vector<int>> connected_components() const {
        return uf.all_groups();
    }

    /**
     * @return 木か
     */
    bool is_tree() const {
        return (uf.count_groups() == 1 && (int)E.size() == N - 1);
    }

    /**
     * @return グラフの重み
     */
    WEIGHT weight() const { return W; }

    /**
     * @param e 辺
     * @attention 渡した辺の id は保持される
     */
    void add_edge(edge<WEIGHT> e) {
        uf.merge(e.from, e.to);
        E.emplace_back(e);
        G[e.from].emplace_back(e);
        if (!is_directed && e.from != e.to) {
            std::swap(e.from, e.to);
            G[e.from].emplace_back(e);
        }
        W += e.cost;
    }

    /**
     * @attention 辺の id は、(現在の辺の本数)番目 が振られる
     * @attention WEIGHT が int だとエラー
     */
    void add_edge(int from, int to, WEIGHT cost) {
        static_assert(!std::is_same<WEIGHT, int>::value);
        if (!is_directed && from > to)
            std::swap(from, to);
        add_edge(edge<WEIGHT>(from, to, cost, E.size()));
    }

    /**
     * @attention 辺の id は、(現在の辺の本数)番目 が振られる
     * @attention WEIGHT が int 以外だとエラー
     */
    void add_edge(int from, int to) {
        static_assert(std::is_same<WEIGHT, int>::value);
        if (!is_directed && from > to)
            std::swap(from, to);
        add_edge(edge<int>(from, to, E.size()));
    }

    /**
     * @brief グラフを連結なグラフに分けてリストにして返す
     * @example auto[Gs, gr, nd] = G.decompose();
     * @returns
     * 1.グラフのリスト
     * 2.各ノードがグラフのリストの何番目に属するか
     * 3.各ノードがグラフのどのノードになっているか
     */
    std::tuple<std::vector<graph>, std::vector<int>, std::vector<int>>
    decompose() const {
        std::vector<graph> Gs(uf.count_groups());
        std::vector<std::vector<int>> groups(uf.all_groups());
        std::vector<int> group_id(N), node_id(N);
        for (int i = 0; i < (int)groups.size(); i++) {
            Gs[i].expand(groups[i].size());
            for (int j = 0; j < (int)groups[i].size(); j++) {
                group_id[groups[i][j]] = i;
                node_id[groups[i][j]] = j;
            }
        }
        for (auto e : E) {
            int id = group_id[e.from];
            e.from = node_id[e.from];
            e.to = node_id[e.to];
            Gs[id].add_edge(e);
        }
        return std::make_tuple(Gs, group_id, node_id);
    }

    /**
     * @brief グラフを隣接行列に変換
     * @param invalid 辺のないときの値
     * @attention G に自己ループが含まれていない限り、対角成分は 0
     */
    matrix<WEIGHT> to_adjajency(WEIGHT invalid = 0) const {
        int N = count_nodes();
        matrix<WEIGHT> ret(N, N, invalid);
        for (int i = 0; i < N; i++)
            ret[i][i] = 0;
        for (int i = 0; i < N; i++)
            for (auto &e : G[i])
                ret[e.from][e.to] = e.cost;
        return ret;
    }

    /**
     * @brief 行きがけ順に bfs
     * @attention グラフは連結であることが必要
     */
    std::vector<int> preorder(int start) const {
        std::vector<int> result;
        std::stack<std::pair<int, int>> stk;
        for (int x : uf.contained_group(start))
            visited[x] = false;
        visited[start] = true;
        stk.push({start, 0});

        while (!stk.empty()) {
            auto &[cu, idx] = stk.top();
            if (idx == 0)
                result.push_back(cu);
            if (idx == G[cu].size()) {
                stk.pop();
            } else {
                int to = G[cu][idx++];
                if (!visited[to]) {
                    visited[to] = true;
                    stk.push({to, 0});
                }
            }
        }
        return result;
    }

    std::vector<int> inorder(int start) const {
        std::vector<int> result;
        std::stack<std::pair<int, int>> stk;
        for (int x : uf.contained_group(start))
            visited[x] = false;
        visited[start] = true;
        stk.push({start, 0});

        while (!stk.empty()) {
            auto &[cu, idx] = stk.top();
            if (idx == G[cu].size()) {
                stk.pop();
                result.push_back(cu);
            } else {
                int to = G[cu][idx++];
                if (!visited[to]) {
                    visited[to] = true;
                    stk.push({to, 0});
                    result.push_back(cu);
                }
            }
        }
        return result;
    }

    std::vector<int> postorder(int start) const {
        std::vector<int> result;
        std::stack<std::pair<int, int>> stk;
        for (int x : uf.contained_group(start))
            visited[x] = false;
        visited[start] = true;
        stk.push({start, 0});

        while (!stk.empty()) {
            auto &[cu, idx] = stk.top();
            if (idx == G[cu].size()) {
                stk.pop();
                result.push_back(cu);
            } else {
                int to = G[cu][idx++];
                if (!visited[to]) {
                    visited[to] = true;
                    stk.push({to, 0});
                }
            }
        }
        return result;
    }

  private:
    using PAIR = std::pair<WEIGHT, int>;
    using Dijkstra_queue =
        std::priority_queue<PAIR, std::vector<PAIR>, std::greater<PAIR>>;

    void run_bfs(std::vector<int> &dist, std::queue<int> &q) const {
        while (!q.empty()) {
            int cu = q.front();
            q.pop();
            for (auto &e : G[cu]) {
                if (dist[e.to] != std::numeric_limits<WEIGHT>::max())
                    continue;
                dist[e.to] = dist[cu] + 1;
                q.push(e.to);
            }
        }
    }

    void run_Dijkstra(std::vector<WEIGHT> &dist, Dijkstra_queue &q) const {
        while (!q.empty()) {
            WEIGHT cur_dist = q.top().first;
            int cu = q.top().second;
            q.pop();

            if (visited[cu])
                continue;
            visited[cu] = true;

            for (auto &e : G[cu]) {
                WEIGHT alt = cur_dist + e.cost;
                if (dist[e.to] <= alt)
                    continue;
                dist[e.to] = alt;
                q.push({alt, e.to});
            }
        }
    }

  public:
    /**
     * @brief 最短距離を計算する
     * @param start_node 始点
     * @param invalid 到達不能な頂点に格納される値
     * @return 各ノードまでの最短距離のリスト
     */
    std::vector<WEIGHT> calculate_shortest_distances(int start_node,
                                                     WEIGHT invalid) const {
        std::vector<WEIGHT> dist(N, std::numeric_limits<WEIGHT>::max());
        dist[start_node] = 0;

        if constexpr (std::is_same<WEIGHT, int>::value) {
            // BFS algorithm
            std::queue<int> q;
            q.push(start_node);
            run_bfs(dist, q);
        } else {
            // Dijkstra's algorithm
            std::priority_queue<PAIR, std::vector<PAIR>, std::greater<PAIR>> q;
            q.push({0, start_node});
            for (int x : uf.contained_group(start_node))
                visited[x] = false;
            run_Dijkstra(dist, q);
        }

        for (auto &x : dist)
            if (x == std::numeric_limits<WEIGHT>::max())
                x = invalid;
        return dist;
    }

    WEIGHT diameter() const {
        static_assert(!is_directed);
        assert(is_tree());
        std::vector<WEIGHT> dist(calculate_shortest_distances(0, -1));
        dist = calculate_shortest_distances(
            std::max_element(dist.begin(), dist.end()) - dist.begin(), -1);
        return *std::max_element(dist.begin(), dist.end());
    }

    graph reverse() const {
        if constexpr (!is_directed) {
            return *this;
        } else {
            graph ret(count_nodes());
            for (auto e : E) {
                std::swap(e.from, e.to);
                ret.add_edge(e);
            }
            return ret;
        }
    }

    /**
     * @return 最小全域森
     */
    graph minimum_spanning_tree() const {
        static_assert(!is_directed);
        graph ret(count_nodes());
        std::vector<edge<WEIGHT>> tmp(edges());
        std::sort(tmp.begin(), tmp.end());
        for (auto &e : tmp)
            if (!ret.are_connected(e.from, e.to))
                ret.add_edge(e);
        return ret;
    }

    void print() const {
        std::cout << this->N << " " << this->E.size() << std::endl;
        for (const edge<WEIGHT> &e : this->E)
            std::cout << e << std::endl;
    }
};
