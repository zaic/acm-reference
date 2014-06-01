// ********************* Match in bipartite graph **********************
const int maxn = 1 << 10;
int who[maxn];
char was[maxn];
vector<int> g[maxn];

bool go(int v){
    if (was[v]) return false;
    was[v] = 1;
    for (int i = 0; i < g[v].size(); ++i) {
        int p = g[v][i];
        if (who[p] < 0 || go(who[p])) {
            who[p] = v;
            return true;
        }
    }
    return false;
}

int max_match(int n) {
    memset(who, -1, sizeof(int) * n);
    int res = 0;
    for (int i = 0; i < n; ++i) {
        memset(was, 0, sizeof(char) * n);
        if (go(i)) ++res;
    }
    return res;
}
