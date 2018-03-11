template<int V, bool DIRECTED>
struct UnweighedGraph {
    vector<int> g[V];

    void isVertex(int v) {
        assert(0 <= v && v < V);
    }
    
    void clear(int to = V) {
        assert(0 <= to && to <= V);

        for (int i = 0; i < to; ++i) g[i].clear();
    }

    void readEdges(int m) {
        while (m --> 0) {
            int v, u; cin >> v >> u;
            addEdge(v, u);
        }
    }

    void addDirecteddEdge(int v, int u) {
        isVertex(v); isVertex(u);

        g[v].push_back(u);
    }

    void addBiEdge(int v, int u) {
        isVertex(v); isVertex(u);

        addDirecteddEdge(v, u);
        addDirecteddEdge(u, v);
    }

    void addEdge(int v, int u) {
        if (DIRECTED)
            addDirecteddEdge(v, u);
        else
            addBiEdge(v, u);
    }

    vector<vector<int>> dfs() {
        vector<vector<int>> comps;
        vector<bool> visit(V, false);
        
        void dfsInternal(int v) {
            visit[v] = true;
            for (int u : g[v]) if (!visit[u]) {
                comps.back().push_back(u);
                dfsInternal(u);
            }
        }

        for (int v = 0; v < V; ++v) {
            comps.push_back(vector<int>(1, v));
            dfs(v);
        }
        return comps;
    }
};

template<int maxv>
struct BFS {
    typedef UnweighedGraph<maxv, false> Graph;

    BFS(const Graph& g) : graph(g) {
    }

    vector<int> run(int start) {
        assert(0 <= start && start < maxv);

        vector<int> dist(maxv, -1);
        queue<int> que;
        que.push(start);
        dist[start] = 0;
        while (!que.empty()) {
            const int v = que.front();
            const int d = dist[v] + 1;
            que.pop();
            for (const int u : graph.g[v]) {
                if (dist[u] == -1) {
                    dist[u] = d;
                    que.push(u);
                }
            }
        }

        return dist;
    }

private:
    const Graph graph;
};
