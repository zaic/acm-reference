template<int maxn>
struct BridgesFinder {
    void addEdge(int v, int u) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<pair<int, int>> run() {
        for (int v = 0; v < maxn; ++v)
            if (!tin[v])
                dfs(v, -1);
        return bridges;
    }

private:
    void addBridge(int v, int u) {
        if (v > u)
            swap(v, u);
        bridges.push_back(make_pair(v, u));
    }

    void dfs(int v, int p) {
        static int number = 1;
        tin[v] = tout[v] = number++;
        for (int u : g[v]) {
            if (u == p) continue; // skip parent

            if (tin[u]) { // is visited
                tout[v] = min(tout[v], tin[u]);
            } else {
                dfs(u, v);
                tout[v] = min(tout[v], tout[u]);
                if (tout[u] > tin[v])
                    addBridge(v, u);
            }
        }
    }

    // input
    vector<int> g[maxn];
    // temporary
    int tin[maxn], tout[maxn];
    // output
    vector<pair<int, int>> bridges;
};
