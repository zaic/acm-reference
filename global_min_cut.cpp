// *************************** Global mincut ***************************
// damages graph!
const int MAXN = 500;
int n, g[MAXN][MAXN];
int best_cost = 1000000000;
vector<int> best_cut;

void mincut() {
	vector<int> v[MAXN];
	for (int i = 0; i < n; ++i) v[i].assign(1, i);
	int w[MAXN];
	bool exist[MAXN], in_a[MAXN];
	memset(exist, true, sizeof exist);
	for (int ph = 0; ph < n - 1; ++ph) {
		memset(in_a, false, sizeof(in_a));
		memset(w, 0, sizeof(w));
		for (int it = 0, prev; it < n - ph; ++it) {
			int sel = -1;
			for (int i = 0; i < n; ++i)
				if (exist[i] && !in_a[i] && (sel == -1 || w[i] > w[sel])) sel = i;
			if (it == n - ph - 1) {
				if (w[sel] < best_cost) best_cost = w[sel],  best_cut = v[sel];
				v[prev].insert(v[prev].end(), v[sel].begin(), v[sel].end());
				for (int i = 0; i < n; ++i) g[prev][i] = g[i][prev] += g[sel][i]; // ?
				exist[sel] = false;
			}
			else {
				in_a[sel] = true;
				for (int i = 0; i < n; ++i) w[i] += g[sel][i]; // ?
				prev = sel;
			}
		}
	}
}
