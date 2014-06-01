// ************************ Hungarian algorithm ************************
const int inf = 0x3f3f3f3f;
int u[maxn], v[maxn], p[maxn], prev[maxn], minv[maxn], nw[maxn];
int c[maxn][maxn];
int n;

int min_cost_assignement() {
	memset(u, 0, sizeof(int) * (n + 1));
	memset(v, 0, sizeof(int) * (n + 1));
	memset(p, 0, sizeof(int) * (n + 1));
	for (int i = 1; i <= n; ++i) {
		int j0 = 0, j1 = -1;
		p[0] = i;
		memset(minv, 0x3f, sizeof(int) * (n + 1));
		minv[0] = 0;
		memset(nw, 0, sizeof(int) * (n + 1));
		do {
			nw[j0] = 1;
			int i0 = p[j0], del = inf;
			for (int j = 1; j <= n; ++j) if (nw[j] == 0) {
				int cc = c[i0 - 1][j - 1] - u[i0] - v[j];
				if (cc < minv[j]) {
					minv[j] = cc;
					prev[j] = j0;
				}
				if (minv[j] < del) {
					del = minv[j];
					j1 = j;
				}
			}
			for (int j = 0; j <= n; ++j)
				if (nw[j] == 1) {
					u[p[j]] += del;
					v[j] -= del;
				} else
					minv[j] -= del;
			j0 = j1;
		} while (p[j0] != 0);
		do {
			j1 = prev[j0];
			p[j0] = p[j1];
			j0 = j1;
		} while (j0 != 0);
	}
	int ans = 0;
	for (int i = 1; i <= n; ++i) ans += c[p[i] - 1][i - 1];
	return ans;
}
