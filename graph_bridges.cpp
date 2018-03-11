template<int maxv>
struct BridgesFinder {
    typedef UnweighedGraph<maxv, false> Graph;

    BridgesFinder(const Graph& _g) : g(_g) {}

    vector<pair<int, int>> findBridges() {
        for (int v = 0; v < maxv; ++v)
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
        for (const int u : g.g[v]) {
            if (!tin[u]) {
                dfs(u, v);
                tout[v] = min(tout[v], tout[u]);
                if (tout[u] > tin[v])
                    addBridge(v, u);

            } else if (u != p) {
                tout[v] = min(tout[v], tin[u]);
            }
        }
    }

    // input
    const Graph g;

    // temporary
    int tin[maxv], tout[maxv];

    // output
    vector<pair<int, int>> bridges;
};

template<int maxv>
struct BridgeCuts {
    typedef UnweighedGraph<maxv, false> Graph;

    BridgeCuts(const Graph& g) : graph(g) {
        memset(visited, 0, sizeof(visited));
        memset(block, 0, sizeof(block));
    }

    Graph run() {
        vector<pair<int, int>> bs = BridgesFinder<maxv>(graph).findBridges();
        for (pair<int, int> e : bs)
            bridges.insert(e);

        int blocks_count = 0;
        for (int v = 0; v < maxv; ++v)
            if (!visited[v])
                dfs(v, blocks_count++);

        Graph result;
        for (auto edge : bridges) {
            const int ba = block[edge.first];
            const int bb = block[edge.second];
            result.addEdge(ba, bb);
        }
        return result;
    }

private:
    void dfs(int v, int bid) {
        visited[v] = true;
        block[v] = bid;
        for (int u : graph.g[v]) {
            if (visited[u] || hasBridge(v, u))
                continue;
            dfs(u, bid);
        }
    }

    bool hasBridge(int v, int u) {
        if (v > u)
            swap(v, u);
        return bridges.count(make_pair(v, u));
    }
    
    // input
    const Graph graph;
    // temporary
    bool visited[maxv];
    int block[maxv]; // map: vertex -> block id
    set<pair<int, int>> bridges;
};
