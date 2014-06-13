// ************************ Hungarian algorithm (min cost) ************************
const double inf = 1e20;
const int maxn = 1010, maxm = 2020;
int prv[maxm], nw[maxm];
double u[maxn], v[maxm], minv[maxm];

int n, m; // number of rows and cols
double c[maxn][maxm]; // input matrix
int p[maxm]; // answer: (p[i]-1) is a row index for column i

double min_cost_assignement() {
	memset(u, 0, sizeof(double) * (n + 1));
	memset(v, 0, sizeof(double) * (m + 1));
	memset(p, 0, sizeof(int) * (m + 1));
    memset(prv, 0, sizeof(int) * (m + 1));
	for (int i = 1; i <= n; ++i) {
		int j0 = 0, j1 = -1;
		p[0] = i;
        for (int j = 0; j <= m; ++j) minv[j] = inf;
		minv[0] = 0;
		memset(nw, 0, sizeof(int) * (m + 1));
		do {
			nw[j0] = 1;
			int i0 = p[j0];
            double del = inf;
			for (int j = 1; j <= m; ++j) if (nw[j] == 0) {
				double cc = c[i0 - 1][j - 1] - u[i0] - v[j];
				if (cc < minv[j]) {
					minv[j] = cc;
					prv[j] = j0;
				}
				if (minv[j] < del) {
					del = minv[j];
					j1 = j;
				}
			}
			for (int j = 0; j <= m; ++j)
				if (nw[j]) {
					u[p[j]] += del;
					v[j] -= del;
				} else
					minv[j] -= del;
			j0 = j1;
		} while (p[j0] != 0);
		do {
			j1 = prv[j0];
			p[j0] = p[j1];
			j0 = j1;
		} while (j0);
	}
	double ans = 0;
	for (int i = 1; i <= m; ++i) if (p[i] > 0) ans += c[p[i] - 1][i - 1];
	return ans;
}
