// ********************* Suffix array construction *********************
char st[maxl];
int ind[maxl], nind[maxl], val[20][maxl], cnt[maxl], cc, log_2, l;

void build_mass() {
	int i, j;
	l = strlen(st);
	for(log_2 = 0; (1 << log_2) < l; ++log_2) ;
	memset(cnt, 0, sizeof(cnt));
	memset(val, 0, sizeof(val));
	for (i = 0; i < l; ++i) cnt[st[i] + 1]++;
	for (i = 0; i < 128; ++i) cnt[i + 1] += cnt[i];
	for (i = 0; i < l; ++i) ind[cnt[st[i]]++] = i;
	for (i = 1; i < l; ++i) val[0][ind[i]] = val[0][ind[i-1]] + (st[ind[i]] != st[ind[i - 1]]);
	for (j = 0; j < log_2; ++j) {
		cnt[0] = 0; cnt[cc = 1] = 1;
		for (i = 1; i < l; ++i)
			if (val[j][ind[i]] == val[j][ind[i - 1]]) ++cnt[cc];
			else cnt[++cc] = 1;
		for (i = 0; i < cc; ++i) cnt[i + 1] += cnt[i];
		int sz = (1 << j);
		for (i = 0; i < l; ++i) nind[ cnt[val[j][(ind[i] - sz + l) % l]]++ ] = (ind[i] - sz + l) % l;
		memcpy(ind, nind, sizeof(int) * l);
		for(i = 1; i < l; ++i)
			val[j + 1][ind[i]] = val[j + 1][ind[i - 1]] + 
			(val[j][ind[i]] != val[j][ind[i - 1]] || val[j][(ind[i] + sz) % l] != val[j][(ind[i - 1] + sz) % l]);
	}
}

int lcp(int i, int j) {
	int ans = 0, k;
	for(k = log_2 - 1; k >= 0; --k)
		if(val[k][i] == val[k][j]) {
			i = (i + (1 << k)) % l;
			j = (j + (1 << k)) % l;
			ans += (1 << k);
		}
	return ans;
}
