// *********************** Aho-Corasik algorithm ***********************
const int maxa = 53;
const int maxn = (3 << 16);

struct node {
	int end, suff, next[maxa];
} nodes[maxn];
int cnt, root;
int q[maxn];
int new_node() {
	memset(nodes + cnt, -1, sizeof(node));
	return cnt++;
}
vector<int> same_words[1 << 10];

void add_word(const char *st, int num) {
	int v;
	for (v = root; *st; ++st) {
		if (nodes[v].next[numc(*st)] == -1) nodes[v].next[numc(*st)] = new_node();
		v = nodes[v].next[numc(*st)];
	}
	if (nodes[v].end == -1) nodes[v].end = num;
	same_words[nodes[v].end].push_back(num);
}

void aho_corasik() { //(only suffix links) automaton construction
	nodes[root].suff = root;
	int l, r = 0, i, u, nx;
	for (i = 0; i < maxa; ++i)
		if ((nx = nodes[root].next[i]) != -1) {
			nodes[nx].suff = root;
			q[r++] = nx;
		} else nodes[root].next[i] = root;
	for (l = 0; l < r; ++l) {
		for (i = 0; i < maxa; ++i) if ((nx = nodes[q[l]].next[i]) != -1) {
			for (u = nodes[q[l]].suff; nodes[u].next[i] == -1; u = nodes[u].suff);
			nodes[nx].suff = nodes[u].next[i];
			q[r++] = nx;
		}
	}
}
int was[maxn], pres[1 << 10];

void search(const char *st) {
	memset(was, 0, sizeof(int) * cnt);
	for (int v = root; *st; ++st) {
		for ( ; nodes[v].next[numc(*st)] == -1; v = nodes[v].suff) ;
		v = nodes[v].next[numc(*st)];
		for (int u = v; u != root; u = nodes[u].suff)
			if (was[u]++) break;
			else if (nodes[v].end != -1)
				for (int i = 0; i < same_words[nodes[v].end].size(); ++i)
					pres[same_words[nodes[v].end][i]] = 1;
	}
}
