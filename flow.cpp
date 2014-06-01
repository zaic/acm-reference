const int N = 300;
int a[N][N], b[N], c[N][N], path[N], n, fin = 1;

void dfs(int v = 0, int d = 0) {
	path[d++] = v;
	b[v] = 1;
	if (b[fin]) return;
	for (int i = 0; i < n; ++i) {
		if ((c[v][i] > 0 || c[i][v] < a[i][v]) && !b[i]) {
			dfs(i, d);
			if (b[fin]) return;
		}
	}
}

bool findPath() {
	for (int i = 0; i < N; ++i) b[i] = 0;
	dfs();
	return (b[fin]);
}

void maxFlow() {
	int i, j;
	for (i = 0; i < n; ++i) for (j = 0; j < n; ++j) c[i][j] = a[i][j];
	while (findPath()) {
		for (i = 0; path[i] != fin; ++i) {
			if (a[ path[i] ][ path[i + 1] ]) --c[ path[i] ][ path[i + 1] ];
			else ++c[ path[i + 1] ][ path[i] ];
		}
	}
}

int main() {
	int i, j, k, m;
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			c[i][j] = a[i][j] = 0;
		}
	}
	cin >> n >> m;
	for (i = 0; i < m; ++i) {
		int x, y, f;
		cin >> x >> y >> f;
		c[x][y] = a[x][y] = f;
	}
	maxFlow();
	/*for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (a[i][j]) {
				printf("[ %d %d ] -> %d / (%d)\n", i, j, a[i][j]-c[i][j], a[i][j]);
			}
		}
	}*/
}
