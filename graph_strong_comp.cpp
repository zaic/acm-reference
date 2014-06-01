// Strongly-connected components
const int maxn = 222;
vector<int> g[maxn]; // <-- input data
int comp[maxn];      // <-- result here ;)

bool visit[maxn];
vector<int> revg[maxn];
vector<int> que, component;
int last_comp_id;

void dfs_fwd(int v) {
    visit[v] = true;
    for (int u : g[v]) if (!visit[u]) dfs_fwd(u);
    que.push_back(v);
}

void dfs_rev(int v) {
    visit[v] = true;
    for (int u : revg[v]) if (!visit[u]) dfs_rev(u);
    component.push_back(v);
}

void tarjan() {
    // add reverse edges
    for (int v = 0; v < maxn; ++v) revg[v].clear();
    for (int v = 0; v < maxn; ++v)
        for (int u : g[v]) 
            revg[u].push_back(v);
    // dfs forward
    memset(visit, 0, sizeof(visit));
    que.clear();
    for (int v = 0; v < maxn; ++v) 
        if (!visit[v]) 
            dfs_fwd(v);
    reverse(que.begin(), que.end());
    // dfs backward
    memset(visit, 0, sizeof(visit));
    for (int v : que) if (!visit[v]) {
        component.clear();
        dfs_rev(v);
        for (int u : component) comp[u] = last_comp_id;
        ++last_comp_id;
    }
}
