template<typename weight_t, int maxv>
struct PreflowMaxFlow {
    const int S = maxv - 2;
    const int T = maxv - 1;
    
    int h[maxv];
    weight_t e[maxv];
    weight_t flow[maxv][maxv], cap[maxv][maxv];

    vector<int> g[maxv];
    int gpos[maxv];

    void reset() {
        memset(cap, 0, sizeof(cap));
        for (int i = 0; i < maxv; ++i) g[i].clear();
        memset(gpos, 0, sizeof(gpos));
    }

    void addDirectedEdge(int from, int to, weight_t ecap) {
        assert(0 <= from && from < maxv);
        assert(0 <= to && to < maxv);

        cap[from][to] += ecap;
        g[from].push_back(to);
        g[to].push_back(from);
    }

    void push(int v, int u) { // push from v to u
        weight_t d = min(e[v], cap[v][u] - flow[v][u]);
        flow[v][u] += d;
        flow[u][v] -= d;
        e[v] -= d;
        e[u] += d;
    }

    void lift(int v) { // up v
        int besth = 0x3f3f3f3f;
        for (int u : g[v]) if (flow[v][u] < cap[v][u])
            besth = min(besth, h[u]);
        assert(besth >= h[v]);
        h[v] = besth + 1;
    }

    void discharge(int v) {
        while (e[v]) {
            if (gpos[v] == int(g[v].size())) {
                lift(v);
                gpos[v] = 0;
                continue;
            }
            int u = g[v][gpos[v]];
            if (flow[v][u] < cap[v][u] && h[v] == h[u] + 1)
                push(v, u);
            else
                ++gpos[v];
        }
    }

    weight_t go() {
        memset(h, 0, sizeof(h));
        h[S] = maxv;
        memset(e, 0, sizeof(e));
        memset(flow, 0, sizeof(flow));
        for (int i = 0; i < S; ++i) if (cap[S][i]) {
            flow[S][i] = e[i] = cap[S][i];
            flow[i][S] = -cap[S][i];
            e[S] -= cap[i][S];
        }
        
        list<int> l;
        for (int i = 0; i < S; ++i) l.push_back(i);
        auto it = l.begin();
        while (it != l.end()) {
            int v = *it;

            int prevh = h[v];
            discharge(v);
            if (h[v] > prevh) {
                l.push_front(v);
                it = l.begin();
            }
            ++it;
        }

        weight_t res = 0;
        for (int v = 0; v < S; ++v) {
            res += flow[S][v];
        }
        return res;
    }
};
