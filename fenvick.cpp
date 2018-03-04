struct {
    int data[maxm];

    void init() {
        memset(data, 0, sizeof(data));
    }

    void upd(int pos, int val) {
        for(; pos < maxm; pos = (pos | (pos+1)))
            data[pos] = max(data[pos], val);
    }

    int get(int pos) {
        int res = 0;
        for(; pos >= 0; pos = (pos & (pos+1)) - 1)
            res = max(res, data[pos]);
        return res;
    }
} fenv;
