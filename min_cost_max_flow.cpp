// ************************* Min-cost max-flow *************************
const int inf = 0x3f3f3f3f;
const int maxn = 1 << 7;
const int maxe = maxn * maxn;
typedef int dist_t;

struct edge {
	int dest, back, f, c;
	dist_t w;
} edges[maxe];
int cnt, n;
vector<int> g[maxn];

void add_edge(int u, int v, int c, dist_t w) {
	edges[cnt].dest = v;
	edges[cnt].back = cnt + 1;
	edges[cnt].f = 0;
	edges[cnt].c = c;
	edges[cnt].w = w;
	g[u].push_back(cnt);
	edges[cnt + 1].dest = u;
	edges[cnt + 1].back = cnt;
	edges[cnt + 1].f = 0;
	edges[cnt + 1].c = 0;
	edges[cnt + 1].w = -w;
	g[v].push_back(cnt + 1);
	cnt += 2;
}
int was[maxn], prev[maxn], r[maxn];
dist_t dist[maxn], phi[maxn];

bool dijk(int s, int t) {
	int i, j;
	for (i = 0; i < n; ++i) dist[i] = inf;
	memset(was, 0, sizeof(int) * n);
	memset(r, 0, sizeof(int) * n);
	dist[s] = 0;
	r[s] = inf;
	for (i = 0; i < n; ++i) {
		int mv = -1;
		for (j = 0; j < n; ++j) if (!was[j] && (mv == -1 || dist[mv] > dist[j])) mv = j;
		if (mv == -1 || dist[mv] == inf) break;
		was[mv] = 1;
		for (j = 0; j < g[mv].size(); ++j) {
			edge &e = edges[ g[mv][j] ];
			if (!was[e.dest] && e.f < e.c && dist[e.dest] > dist[mv] + e.w + phi[mv] - phi[e.dest]) {
				dist[e.dest] = dist[mv] + e.w + phi[mv] - phi[e.dest];
				r[e.dest] = min(r[mv], e.c - e.f);
				prev[e.dest] = e.back;
			}
		}
	}
	return r[t] > 0;
}

dist_t aug(int s, int t) {
	int rr = r[t];
	dist_t ans = 0;
	while (s != t) {
		edge &e = edges[prev[t]];
		ans -= e.w * rr;
		e.f -= rr;
		edges[e.back].f += rr;
		t = e.dest;
	}
	return ans;
}
int flow;
dist_t cost;

void min_cost_max_flow(int s, int t) {
	int i;
	for (i = 0; i < n; ++i) phi[i] = 0;
	cost = flow = 0;
	while (dijk(s, t)) {
		flow += r[t];
		cost += aug(s, t);
		for (i = 0; i < n; ++i) phi[i] += dist[i];
	}
}
