template<int maxn>
struct ArticulationsFinder {
    void addEdge(int v, int u) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    set<int> run() {
        for (int v = 0; v < maxn; ++v)
            if (!tin[v])
                dfs(v, -1);
        return artPoints;
    }

private:
    void dfs(int v, int p) {
        static int number = 1;
        tin[v] = tout[v] = number++;
        int children = 0;
        for (int u : g[v]) {
            if (u == p) continue; // skip parent

            if (tin[u]) { // is visited
                tout[v] = min(tout[v], tin[u]);
            } else {
                ++children;
                dfs(u, v);
                tout[v] = min(tout[v], tout[u]);
                if (tout[u] >= tin[v] && p != -1)
                    artPoints.insert(v);
            }
        }
        if (p == -1 && children > 1)
            artPoints.insert(v);
    }

    // input
    vector<int> g[maxn];
    // temporary
    int tin[maxn], tout[maxn];
    // output
    set<int> artPoints;
};
