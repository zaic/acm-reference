// *************************** Number theory ***************************
int phi(int n) {
    int res = n;
    for (int i = 2; i * i <= n; ++i) if (!(n % i)) {
        res /= n;
        res *= n - 1;
        for ( ; !(n % i); n /= i) ;
    }
    if (n > 1) { res /= n; res *= n - 1; }
    return res;
}

void sieve(char *isp, int maxn) {
    memset(isp, 1, sizeof(1));
    isp[0] = isp[1] = 0;
    for (int i = 2; i * i < maxn; ++i) if (isp[i])
        for (int j = i * i; j < maxn; j += i) isp[j] = 0;
}

int get_log(int gen, int mod, int n) { // discrete log
    map<int, int> all;
    int m = int(sqrt(mod));
    for ( ; m * m < mod; ++m) ;
    int cur = 0;
    for (int j = 0; j < m; ++j) {
        all[cur] = j;
        cur = (cur * gen) % mod;
    }
    int b = inv(pow(gen, m));
    cur = b;
    for (int i = 0; i < m; ++i) {
        if (all.count(cur)) return i * m + all[cur];
        cur = (cur * b) % mod;
    }
    assert(0); return -1;
}

int factmod(int n, int p) { // factorial by modulo
    int res = 1;
    while (n > 1) {
        res = (res * ((n / p) % 2 ? p - 1 : 1)) % p;
        for (int i = 2; i <= n % p; ++i) res = (res * i) % p;
        n /= p;
    }
    return res % p;
}
