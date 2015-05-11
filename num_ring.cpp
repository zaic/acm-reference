template<int64_t MOD, typename T = int, typename TCAST = int64_t>
struct FieldElement {
    T value;

    void norm() {
        value %= MOD;
        if (value < 0) value += MOD;
    }

    static T fastPow(TCAST val, int64_t pw) {
        TCAST res = 1;
        for (; pw; pw >>= 1) {
            if (pw&1)
                res = (res * val) % MOD;
            val = (val * val) % MOD;
        }
        return res;
    }

    FieldElement() : value(0) { }
    FieldElement(const TCAST& val) : value(val) { norm(); }
    FieldElement(const FieldElement& other) : value(other.value) { }
    
    FieldElement& operator=(const FieldElement& other) {
        value = other.value;
        return *this;
    }

    FieldElement& operator+=(const FieldElement& other) {
        value += other.value;
        if (value >= MOD) value -= MOD;
        return *this;
    }

    FieldElement operator+(const FieldElement& other) const {
        FieldElement res(value);
        return res += other;
    }

    FieldElement& operator-=(const FieldElement& other) {
        value = value - other.value;
        if (value < 0) value += MOD;
        return *this;
    }

    FieldElement operator-(const FieldElement& other) const {
        FieldElement res(value);
        res -= other;
        return res;
    }

    FieldElement& operator*=(const FieldElement& other) {
        value = TCAST(value) * other.value % MOD;
        return *this;
    }

    FieldElement operator*(const FieldElement& other) const {
        FieldElement res(*this);
        res *= other;
        return res;
    }

    FieldElement& operator/=(const FieldElement& other) {
        assert(other.value > 0);
        value = (TCAST(value) * fastPow(other.value, MOD - 2)) % MOD;
        return *this;
    }

    FieldElement operator/(const FieldElement& other) {
        FieldElement res(value);
        res /= other;
        return res;
    }
};
typedef FieldElement<int(1e9)+7> m64;
