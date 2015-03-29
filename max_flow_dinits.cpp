template<typename weight_t, int maxv, int maxe>
struct DinitsMaxFlow {
    const int S = maxv - 1;
    const int T = maxv - 2;

    struct Edge {
        int to;
        weight_t cap, flow;
    } edges[maxe];
    int ecount;

    vector<int> g[maxv];
    int dist[maxv];
    int sid[maxv];

    void reset() {
        ecount = 0;
        for (int i = 0; i < maxv; ++i)
            g[i].clear();
    }

    void addDirectedEdge(int from, int to, weight_t cap) {
        assert(0 <= from && from < maxv);
        assert(0 <= to && to < maxv);
        assert(ecount + 2 <= maxe);

        edges[ecount].to = to;
        edges[ecount].cap = cap;
        edges[ecount].flow = 0;
        g[from].push_back(ecount);
        ecount++;
        edges[ecount].to = from;
        edges[ecount].cap = 0;
        edges[ecount].flow = 0;
        g[to].push_back(ecount);
        ecount++;
    }

    int que[maxv];
    bool bfs() {
        memset(dist, 0x3f, sizeof(dist));
        int fr = 0, bc = 1;
        dist[S] = 0;
        que[0] = S;
        while (fr < bc) {
            int v = que[fr++];
            for (int eid = 0; eid < int(g[v].size()); ++eid) {
                const int e = g[v][eid];
                int u = edges[e].to;
                if (edges[e].flow < edges[e].cap && dist[u] > dist[v] + 1) {
                    dist[u] = dist[v] + 1;
                    que[bc++] = u;
                }
            }
        }
        return (dist[T] < 0x3f3f3f3f);
    }

    weight_t dfs(int v, weight_t flow) {
        if (v == T || flow == 0) return flow;
        for (; sid[v] < int(g[v].size()); ++sid[v]) {
            int e = g[v][sid[v]];
            int u = edges[e].to;
            if (dist[u] == dist[v] + 1 && edges[e].flow < edges[e].cap) {
                flt res = dfs(u, min(flow, edges[e].cap - edges[e].flow));
                if (res > 0) {
                    edges[e].flow += res;
                    edges[e ^ 1].flow -= res;
                    return res;
                }
            }
        }
        return 0;
    }

    flt go() {
        flt res = 0;
        while (bfs()) {
            memset(sid, 0, sizeof(sid));
            while (true) {
                flt cur = dfs(S, weight_t(1e9));
                if (cur == 0) break;
                res += cur;
            }
        }
        return res;
    }
    
};
