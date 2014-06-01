// ************************* Ukkonen algorithm *************************
const int maxa = 53;
const int maxl = 1 << 17;
char s[maxl];

struct node {
	int k, p, suff;
	int next[maxa];
} tree[maxl * 2];
int cnt, root, aux;
int new_node(int k, int p) {
	memset(tree + cnt, -1, sizeof(node));
	tree[cnt].k = k; tree[cnt].p = p;
	return cnt++;
}

bool test_and_split(int v, int k, int p, char c, int &r) {
	if (k >= p) { r = v; return tree[v].next[numc(c)] == -1; }
	int nx = tree[v].next[ numc(s[k]) ];
	char wc = s[tree[nx].k + p - k];
	if (wc == c) { r = v; return false; }
	r = new_node(tree[nx].k, tree[nx].k + p - k);
	tree[v].next[numc(s[k])] = r;
	tree[r].next[numc(wc)] = nx;
	tree[nx].k += p - k;
	return true;
}

void canonize(int& v, int& k, int p) {
	while (k < p) {
		int nx = tree[v].next[numc(s[k])];
		if (tree[nx].p - tree[nx].k > p - k) break;
		k += tree[nx].p - tree[nx].k;
		v = nx;
	}
}

void make_tree() { // You MUST ensure to add '$' at the end of the string
	int oldr, v, k, p, i, len = strlen(s), r;
	cnt = 0;
	aux = new_node(-1, -1);
	v = root = new_node(-1, 0);
	for (i = 0; i < maxa; ++i) tree[aux].next[i] = root;
	tree[root].suff = aux;
	for (i = k = 0; i < len; ++i) {
		for (oldr = root; test_and_split(v, k, i, s[i], r); canonize(v, k, i)) {
			tree[r].next[numc(s[i])] = new_node(i, len);
			if (oldr != root) tree[oldr].suff = r;
			oldr = r;
			v = tree[v].suff;
		}
		if (oldr != root) tree[oldr].suff = r;
		canonize(v, k, i + 1);
	}
}

bool find_str(const char *st) {
	for (int v = root, k = 0; *st; ++st) {
		if (k >= tree[v].p) {
			if (tree[v].next[numc(*st)] == -1) return false;
			v = tree[v].next[numc(*st)];
			k = tree[v].k;
		}
		if (s[k++] != *st) return false;
	}
	return true;
}
