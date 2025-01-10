#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>

using namespace std;

class UnionFind {
private:
    vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    bool unite(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);
        if (root_u != root_v) {
            if (rank[root_u] > rank[root_v]) {
                parent[root_v] = root_u;
            } else if (rank[root_u] < rank[root_v]) {
                parent[root_u] = root_v;
            } else {
                parent[root_v] = root_u;
                rank[root_u]++;
            }
            return true;  // Successfully united
        }
        return false;  // Already in the same component
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, T, Q;
    cin >> N >> T >> Q;

    vector<pair<int, int>> roads(T);
    for (int i = 0; i < T; ++i) {
        cin >> roads[i].first >> roads[i].second;
        roads[i].first--; // Convert to 0-based index
        roads[i].second--;
    }

    vector<tuple<int, int, int>> queries(Q);
    for (int i = 0; i < Q; ++i) {
        int s, t;
        cin >> s >> t;
        s--; t--; // Convert to 0-based index
        queries[i] = {s, t, i};
    }

    // To store the earliest turn each query pair becomes connected
    vector<int> answers(Q, -1);
    UnionFind uf(N);

    // Map each query to its turn once they are connected
    unordered_map<int, vector<int>> query_map;

    // Initialize query_map with each query index for later reference
    for (int i = 0; i < Q; ++i) {
        int s = get<0>(queries[i]);
        int t = get<1>(queries[i]);
        if (s != t) {
            int root_s = uf.find(s);
            int root_t = uf.find(t);
            if (root_s != root_t) {
                query_map[i].push_back(i);
            }
        }
    }

    // Process each road connection in order and check connectivity for queries
    for (int i = 0; i < T; ++i) {
        int u = roads[i].first;
        int v = roads[i].second;
        if (uf.unite(u, v)) {
            int root_u = uf.find(u);
            int root_v = uf.find(v);

            // For any pair in query_map that now has the same root, record turn
            for (const auto& [q_idx, indices] : query_map) {
                int s = get<0>(queries[q_idx]);
                int t = get<1>(queries[q_idx]);
                if (uf.find(s) == uf.find(t)) {
                    answers[q_idx] = i + 1;
                    query_map.erase(q_idx);
                }
            }
        }
    }

    // Output answers
    for (int ans : answers) {
        cout << ans << "\n";
    }

    return 0;
}
