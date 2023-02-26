template<class IntegerType>
struct TRatio {
	IntegerType up, down;

	explicit TRatio(IntegerType u) : up(u), down(1) {}

	TRatio(IntegerType u, IntegerType d) {
		assert(d != 0);

		const IntegerType g = gcd(u, d);
		up = u / g;
		down = d / g;
		if (down < 0) {
			up *= -1;
			down *= -1;
		}
	}

	IntegerType gcd(IntegerType a, IntegerType b) {
		a = abs(a);
		b = abs(b);
		while (b) {
			a %= b;
			swap(a, b);
		}
		return a;
	}


	TRatio operator*(int64_t value) const {
		return TRatio(up * value, down);
	}


	TRatio operator+(const TRatio& other) const {
		return TRatio(up * other.down + other.up * down, down * other.down);
	}

	TRatio operator-(const TRatio& other) const {
		return TRatio(up * other.down - other.up * down, down * other.down);
	}

	TRatio operator*(const TRatio& other) const {
		return TRatio(up * other.up, down * other.down);
	}

	TRatio operator/(const TRatio& other) const {
		return TRatio(up * other.down, down * other.up);
	}


	bool operator<(const TRatio& other) const {
		return up * other.down < other.up* down;
	}

	bool operator<=(const TRatio& other) const {
		return up * other.down <= other.up * down;
	}
};

template<class T>
ostream& operator<<(ostream& os, const TRatio<T>& ratio) {
	return os << ratio.up << "/" << ratio.down << "~(" << double(ratio.up) / ratio.down << ")";
}