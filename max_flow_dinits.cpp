#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <stack>
#include <algorithm>
#include <ios>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <set>
#include <functional>
#include <cmath>
#include <sstream>
#include <map>
#include <memory.h>
#include <stdio.h>
#include <cassert>
#include <string.h>
#include <deque>

#define forn(i , n) for (int i = 0; i < n; ++i)
#define down(i, n) for (int i = n - 1; i >= 0; --i)


using namespace std;

typedef unsigned long long int u64;
typedef long long int i64;
typedef vector<int> vint;
typedef vector<i64> vi64;
typedef pair<int, int> pint;
typedef pair<i64, i64> pi64;
typedef double flt;

#define FILE_NAME "file"
#define CONTEST "seq"
#define M_PI 3.14159265358979323846

#define E(x) { cerr << #x << " = " << (x) << "   "; }
#define Eo(x) { cerr << #x << " = " << (x) << endl; }
#define EO(x) Eo(x)



const i64 inf = 100000000000000000LL;

#define MOD 10000000000007 

const int maxn = 205;
const flt eps = 1e-4;

int n, m, w, h;
flt minval[202][202];
flt maxval[202][202];
flt wi[maxn];
flt have[maxn];
flt curh[maxn];

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
DinitsMaxFlow<flt, 410, 2 * (202 * 202 + 500)> myflow;

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout << fixed << setprecision(15);
	srand(23);

#ifdef FILE_INPUT
	freopen(FILE_NAME ".in", "r", stdin);
	freopen(FILE_NAME ".out", "w", stdout);
#endif

	cin >> n >> m >> w >> h;
	for (int i = 0; i < m; ++i) cin >> have[i];
	flt lastx = 0;
	for (int i = 0; i < n - 1; ++i) {
		flt nextx; cin >> nextx;
		wi[i] = nextx - lastx;
		lastx = nextx;
	}
	wi[n - 1] = w - lastx;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			cin >> minval[i][j];
			//E(i); E(j); Eo(minval[i][j]);
		}
	}

	for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) cin >> maxval[i][j];

	flt maxh = 0, minh = 5050;
	for (int i = 0; i < n; ++i) {
		flt sumv = 0;
		for (int j = 0; j < m; ++j) {
			sumv += minval[i][j];
			have[j] -= minval[i][j];
		}
		curh[i] = sumv / wi[i];
		maxh = max(maxh, curh[i]);
		minh = min(minh, curh[i]);
		//E(i); EO(curh[i]);
	}
	//E(minh); Eo(maxh);

	flt l = minh, r = maxh;
	for (int iter = 0; iter < 30; ++iter) {
		flt mid = (l + r) / 2;

        myflow.reset();
		flt sumreq = 0;
		for (int i = 0; i < n; ++i) if (curh[i] < mid) {
			flt reqh = mid - curh[i];
			flt reqv = reqh * wi[i];
			sumreq += reqv;
            myflow.addDirectedEdge(m + i, myflow.T, reqv);

			for (int j = 0; j < m; ++j) myflow.addDirectedEdge(j, m + i, maxval[i][j] - minval[i][j]);
		}
		for (int i = 0; i < m; ++i) myflow.addDirectedEdge(myflow.S, i, have[i]);

		flt flowSize = 0;
        flt cur = myflow.go();
        flowSize += cur;

		if (flowSize + 1e-4 < sumreq)
			r = mid;
		else
			l = mid;
	}

	printf("%.3lf\n", double(maxh - l));
	
	return 0;
}
