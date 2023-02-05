// ***************** Setting the line up in the bitset *****************
inline const bool test(dword *arr, const int &x) {
	return (arr[x >> 5] >> (x & 31)) & 1;
}
inline void setBit(dword *arr, const int &x) {
	arr[x >> 5] |= (1 << (x & 31));
}
inline const dword getMask(const int &l, const int &r) {
	return (r < 32 ? (dword(1) << r) : dword(0)) - (dword(1) << l);
}
inline void setLine(dword *arr, int l, int r) {
	if (l > r) return;
	int li = (l >> 5);
	int ri = (r >> 5);
	if (li == ri) {
		arr[li] |= getMask(l - (li << 5), r + 1 - (ri << 5));
		return;
	}
	arr[li] |= getMask(l - (li << 5), 32);
	arr[ri] |= getMask(0, r + 1 - (ri << 5));
	for (int i = li + 1; i <= ri - 1; ++i) arr[i] = dword(-1);
}

// **************************** all submasks ***************************
for (int s = m; ;s = (s - 1) & m) {
    do_smth(s);
    if (!s) break;
}

// ***************************** Geometry ******************************
struct Point { //integer point
	int x, y;
	inline Point() : x(0), y(0) {}
	inline Point(int _x, int _y) : x(_x), y(_y) {}
	inline int len2() const { return x * x + y * y; }
    inline int operator!() const { return x*x + y*y; }
	inline const Point operator + (const Point &b) const {
		return Point(x + b.x, y + b.y);
	}
	inline const Point operator - (const Point &b) const {
		return Point(x - b.x, y - b.y);
	}
	inline const Point operator * (int b) const {
		return Point(x * b, y * b);
	}
	inline bool operator == (const Point &b) const {
		return x == b.x && y == b.y;
	}
	inline bool half() const {
		return (y < 0 || (y == 0 && x < 0));
	}
	inline bool operator < (const Point &b) const;
};
inline int vect(const Point &a, const Point &b) {
	return a.x * b.y - a.y * b.x;
}
inline int scal(const Point &a, const Point &b) {
	return a.x * b.x + a.y * b.y;
}

// comparing by polar angle, <*this> and <b> MUST BE NONZERO
inline bool Point::operator < (const Point &b) const {
	int th = half();
	int bh = b.half();
	if (th ^ bh) return th < bh;
	int pv = vect(*this, b);
	return pv > 0;
}

// decides whether <p> is inside the oriented angle (<a>-<b>) (including bounds)
// both <a> and <b> are nonzero
bool inSector(const Point &a, const Point &b, const Point &p) {
	int vab = vect(a, b);
	if (!vab && (scal(a, b) > 0)) if (scal(a, p) < 0) return false;
	if (vab >= 0) return (vect(a, p) >= 0 && vect(p, b) >= 0);
	else		  return (vect(a, p) >= 0 || vect(p, b) >= 0);
}

inline bool CrossSegs(int l1, int r1, int l2, int r2) {
	if (l1 > r1) swap(l1, r1);
	if (l2 > r2) swap(l2, r2);
	return !(l1 > r2 || r1 < l2);
}

// crosses two closed line segments <p1>-<p2> and {p3>-<p4>
bool CrossLineLine(const Point &p1, const Point &p2, const Point &p3, const Point &p4) {
	int a11 = p2.x - p1.x;
	int a12 = p3.x - p4.x;
	int a21 = p2.y - p1.y;
	int a22 = p3.y - p4.y;
	int b1  = p3.x - p1.x;
	int b2  = p3.y - p1.y;
	int det  = a11 * a22 - a12 * a21;
	int detu =  b1 * a22 - a12 *  b2;
	int detv = a11 *  b2  - b1 * a21;
	if (det == 0) {
		if (detu || detv) return false;
		if (p1.x != p2.x || p3.x != p4.x) return CrossSegs(p1.x, p2.x, p3.x, p4.x);
		if (p1.y != p2.y || p3.y != p4.y) return CrossSegs(p1.y, p2.y, p3.y, p4.y);
		return (p1 == p3);
	}
	if (det < 0) {
		det = -det;
		detu = -detu;
		detv = -detv;
	}
	return (detu >= 0 && detu <= det && detv >= 0 && detv <= det);
}

// determines whether point <p> lies on the closed segment <a>-<b>
inline bool onLine(const Point &p, const Point &a, const Point &b) {
	if (vect(p - a, b - a)) return false;
	if (a == b) return p == a;
	return (scal(p - a, b - a) >= 0 && scal(p - b, a - b) >= 0);
}

// determines whether a point <p> is inside the triangle <a>-<b>-<c>
// does not work for triangles with zero area
inline int uabs(int a) { return (a < 0 ? -a : a); }
inline bool inTriangle(const Point &p, const Point &a, const Point &b, const Point &c, bool strict = false) {
	int tv = uabs(vect(c - a, b - a));
	int t = uabs(vect(p - a, b - a));
	if (strict && !t) return false;
	tv -= t;
	t = uabs(vect(p - b, c - b));
	if (strict && !t) return false;
	tv -= t;
	t = uabs(vect(p - c, a - c));
	if (strict && !t) return false;
	tv -= t;
	return tv >= 0;
}

// determines whether point <p> lies inside polygon <arr[0], ..., arr[n]>
// no self-crossings!	may have equal points	arr[0] = arr[n]
bool inPolygon(const Point &p, int n, const Point *arr, bool strict = false) {
    //	if lies on the border
	for (int i = 0; i < n; ++i) if (onLine(p, arr[i], arr[i + 1])) return !strict;
	Point spot = p + Point(15013, 15017); // BIG PRIMES: 1061109589 / 1061109601
	int cnt = 0;
	for (int i = 0; i < n; ++i) if (CrossLineLine(spot, p, arr[i], arr[i + 1])) cnt ^= 1;
	return bool(cnt);
}

// Graham's convex hull
// changes order of points! no equal points allowed! must have nonzero area
Point hctr;
bool cmpHull(const Point &a, const Point &b) {
    Point ad = a - hctr;
    Point bd = b - hctr;
    int tv = vect(ad, bd);
    if (tv) return tv > 0;
    return ad.len2() < bd.len2();
}
void ConvexHull(int n, Point *arr, int &k, Point *res, bool strict = true) {
    int i;
    int best = 0;
    for (i = 1; i<n; i++) {
        if (arr[i].x < arr[best].x) best = i;
        if (arr[i].x == arr[best].x && arr[i].y < arr[best].y) best = i;
    }
    std::swap(arr[best], arr[0]);
    hctr = arr[0];
    std::sort(arr+1, arr+n, cmpHull);
    k = 0;
    res[k++] = arr[0];
    res[k++] = arr[1];
    for (i = 2; i<n; i++) {
        if (strict) while (k>=2 && vect(res[k-1]-res[k-2], arr[i]-res[k-2]) <= 0) k--;
        if (!strict) while (k>=2 && vect(res[k-1]-res[k-2], arr[i]-res[k-2]) < 0) k--;
        res[k++] = arr[i];
    }
    if (!strict) {
        k--;
        for (i = n-1; i>0; i--) {
            Page 18 of 25
                res[k++] = arr[i];
            if (vect(arr[i]-arr[0], arr[i-1]-arr[0]) != 0) break;
        }
    }
}


//real_t point
//Line crosses Line (infinite)
bool CrossLineLine(const Point &a1, const Point &b1, const Point &a2, const Point &b2, Point &res) {
	real_t a11 = b1.x - a1.x;
	real_t a12 = a2.x - b2.x;
	real_t a21 = b1.y - a1.y;
	real_t a22 = a2.y - b2.y;
	real_t xb1 = a2.x - a1.x;
	real_t xb2 = a2.y - a1.y;
	real_t det  = a11 * a22 - a12 * a21;
	real_t detu = xb1 * a22 - a12 * xb2;
	real_t detv = a11 * xb2 - xb1 * a21;
	if (abs(det) < EPS) return false;
	detu /= det;
	detv /= det;
	Point c = a1 + (a2 - a1) * detu;
	res = c;
	return true;
}

//Line crosses Circle (infinite)
bool CrossLineCircle(const Point &la, const Point &lb, const Point &cc, real_t cr, Point &res1, Point &res2) {
	Point st = la - cc;
	Point dir = lb - la;
	real_t qa = scal(dir, dir);
	real_t qb = 2.0 * scal(st, dir);
	real_t qc = scal(st, st) - cr * cr;
	real_t qd = qb * qb - 4.0 * qa * qc;
	if (qd < -EPS) return false;
	if (qd < 0.0) qd = 0.0;
	qd = sqrt(qd);
	real_t x1 = (-qb - qd) / (2.0 * qa);
	real_t x2 = (-qb + qd) / (2.0 * qa);
	res1 = la + dir * x1;
	res2 = la + dir * x2;
	return true;
}

//Circle crosses Circle
bool CrossCircleCircle(const Point &c1, real_t r1, const Point &c2, real_t r2, Point &res1, Point &res2) {
	real_t la = 2.0 * (c2.x - c1.x);
	real_t lb = 2.0 * (c2.y - c1.y);
	real_t lc = sqr(c1.x) - sqr(c2.x) + sqr(c1.y) - sqr(c2.y) + sqr(r2) - sqr(r1);
	if (la * la + lb * lb < EPS) return false;
	Point a, b;
	if (abs(la) > abs(lb)) {
		a = Point(-lc / la, 0.0);
		b = Point(-(lb + lc) / la, 1.0);
	}
	else {
		a = Point(0.0, -lc / lb);
		b = Point(1.0, -(lc + la) / lb);
	}
	return CrossLineCircle(a, b, c1, r1, res1, res2);
}

// ******************************* Gauss *******************************
int n, m, r;
real_t matr[SIZE][SIZE];
int adr[SIZE];
bool used[SIZE];
real_t sol[SIZE];
void Gauss() {
	int i, j, u;
	r = 0;
	memset(used, 0, sizeof(used));
	for (i = 0; i <= n; ++i) {
		int best = -1;
		for (j = r; j < m; ++j) if (best < 0 || abs(matr[j][i]) > abs(matr[best][i])) best = j;
		if (best < 0) break;
		for (u = 0; u <= n; ++u) swap(matr[best][u], matr[r][u]);
		if (abs(matr[r][i]) < EPS) continue;
		for (u = n; u >= i; --u) matr[r][u] /= matr[r][i];
		for (j = 0; j < m; ++j) if (j != r) {
			real_t coef = matr[j][i];
			for (u = i; u <= n; ++u) matr[j][u] -= coef * matr[r][u];
		}
		used[i] = true;
		adr[r++] = i;
	}
}
bool GetSolution() { // gets the solution
	int i, j;
	memset(sol, 0, sizeof(sol));
	if (used[n]) return false;
	sol[n] = -1.0;	//MUST BE SO!
	for (i = 0; i < n; ++i)
		if (!used[i]) sol[i] = rand() / 32768.0; //free variables
	for (i = r - 1; i >= 0; --i)
		for (j = adr[i] + 1; j <= n; ++j)
			sol[adr[i]] -= sol[j] * matr[i][j];
	return true;
}

// ********************* Integer polynom dividing **********************
////1
struct Poly {
	int deg;
	int arr[SIZE];
	Poly Divide(const Poly &b) { //Divides poly by poly
		Poly ost = *this;
		Poly bad; bad.deg = -1;	//Bad poly with negative degree
		Poly res; res.deg = deg - b.deg;
		int i, j;
		for (i = deg - b.deg; i >= 0; --i) {
			if (ost.arr[i + b.deg] % b.arr[b.deg]) return bad;
			int coef = ost.arr[i + b.deg] / b.arr[b.deg];
			res.arr[i] = coef;
			for (j = 0; j <= b.deg; ++j) ost.arr[i + j] -= coef * b.arr[j];
		}
		//checking for even division; delete if reminder is needed
		for (i = 0; i < b.deg; ++i) if (ost.arr[i]) return bad;
		return res;
	}
};

// ******************************** LCA ********************************
// LCA of two nodes: <a> and <b>, needs (N*logN)*sizeof(int) memory
int n;
int hgt[SIZE];
int father[LOGS][SIZE];
void LCAInit() {
	//init array <hgt> - heights of nodes
	//init array <father[0]> - fathers of nodes (-1 if none)
	int i, j;
	for (i = 1; i < LOGS; ++i) {
		for (j = 0; j < n; ++j) {
			if (father[i - 1][j] < 0) father[i][j] = -1;
			else father[i][j] = father[i - 1][ father[i - 1][j] ];
		}
	}
}
int LCA(int a, int b) {
	if (hgt[a] < hgt[b]) swap(a, b);
	int i;
	for (i = LOGS - 1; i >= 0; --i)
		if (father[i][a] >= 0 && hgt[father[i][a]] >= hgt[b]) a = father[i][a];
	for (i = LOGS - 1; i >= 0; --i) if (father[i][a] != father[i][b]) {
		a = father[i][a];
		b = father[i][b];
	}
	if (a != b) a = father[0][a];
	return a;
}

// ******************************* Levit *******************************
vector<int> d(n, inf);
d[v1] = 0;
vector<int> id(n);
deque<int> q;
q.push_back(v1);
vector<int> p(n, -1);
while (!q.empty()) {
	int v = q.front();
	q.pop_front();
	id[v] = 1;
	for (size_t i = 0; i < g[v].size(); ++i) {
		int to = g[v][i].first, len = g[v][i].second;
		if (d[to] > d[v] + len) {
			d[to] = d[v] + len;
			if (id[to] == 0) q.push_back(to);
			else if (id[to] == 1) q.push_front(to);
			p[to] = v;
			id[to] = 1;
		}
	}
}

// **************************** maxflow:lift ****************************
int n, s, t;
vector < vector<int> > c(n, vector<int>(n));
vector<int> e(n);
vector<int> h(n);
h[s] = n - 1;
vector< vector<int> > f(n, vector<int>(n));

for (int i = 0; i < n; ++i) {
	f[s][i] = c[s][i];
	f[i][s] = -f[s][i];
	e[i] = c[s][i];
}
vector<int> maxh (n);
int sz = 0;
for (;;) {
	if (!sz)
		for (int i = 0; i < n; ++i) if (i != s && i != t && e[i] > 0) {
			if (sz && h[i] > h[maxh[0]]) sz = 0;
			if (!sz || h[i] == h[maxh[0]]) maxh[sz++] = i;
		}
	if (!sz) break;
	while (sz) {
		int i = maxh[sz - 1];
		bool pushed = false;
		for (int j = 0; j < n && e[i]; ++j)
			if (c[i][j] - f[i][j] > 0 && h[i] == h[j] + 1) {
				pushed = true;
				int addf = min(c[i][j] - f[i][j], e[i]);
				f[i][j] += addf, f[j][i] -= addf;
				e[i] -= addf, e[j] += addf;
				if (e[i] == 0) --sz;
			}
		if (!pushed) {
			h[i] = INF;
			for (int j = 0; j < n; ++j)
				if (c[i][j] - f[i][j] > 0 && h[j] + 1 < h[i]) h[i] = h[j] + 1;
			if (h[i] > h[maxh[0]]) {
				sz = 0;
				break;
			}
		}
	}
}

// *************************** maxflow:Dinits ***************************
int n, c[MAXN][MAXN], f[MAXN][MAXN], s, t, d[MAXN], ptr[MAXN], q[MAXN];

bool bfs() {
	int qh=0, qt=0;
	q[qt++] = s;
	memset(d, -1, n * sizeof(d[0]));
	d[s] = 0;
	while (qh < qt) {
		int v = q[qh++];
		for (int to = 0; to < n; ++to) if (d[to] == -1 && f[v][to] < c[v][to]) {
			q[qt++] = to;
			d[to] = d[v] + 1;
		}
	}
	return d[t] != -1;
}
int dfs (int v, int flow) {
	if (!flow) return 0;
	if (v == t) return flow;
	for (int &to = ptr[v]; to < n; ++to) {
		if (d[to] != d[v] + 1)  continue;
		int pushed = dfs(to, min(flow, c[v][to] - f[v][to]));
		if (pushed) {
			f[v][to] += pushed;
			f[to][v] -= pushed;
			return pushed;
		}
	}
	return 0;
}
int dinic() {
	int flow = 0;
	for (;;) {
		if (!bfs()) break;
		memset(ptr, 0, n * sizeof ptr[0]);
		while (int pushed = dfs (s, INF)) flow += pushed;
	}
	return flow;
}
