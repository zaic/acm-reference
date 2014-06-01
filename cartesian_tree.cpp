// ******************* Cartesian tree implementation *******************
typedef int fkey_t;
typedef int skey_t;

struct node {
    fkey_t pk;
    skey_t sk;
    int sz;
    node *left, *right;
    int val, min_val, max_val;
    int64 sum_val;
    int sum, rev;
    void recalc();
    int size() {
        return this == NULL ? 0 : sz;
    }
};

void node::recalc() {
    sz = 1 + left->size() + right->size();
    sum_val = min_val = max_val = (val += sum);
    if (left != NULL) {
        left->sum += sum;
        left->rev ^= rev;
        min_val = min(min_val, left->min_val + left->sum);
        max_val = max(max_val, left->max_val + left->sum);
        sum_val += left->sum_val + int64(left->sum) * left->size();
    }
    if (right != NULL) {
        right->sum += sum;
        right->rev ^= rev;
        min_val = min(min_val, right->min_val + right->sum);
        max_val = max(max_val, right->max_val + right->sum);
        sum_val += right->sum_val + int64(right->sum) * right->size();
    }
    if (rev) swap(left, right);
    sum = rev = 0;
}
node *root = NULL;

node* new_node(const fkey_t &pk, const skey_t &sk) {
    node* ptr = new node();
    ptr->pk = pk; ptr->sk = sk;
    ptr->left = ptr->right = NULL;
    ptr->sz = 1;
    ptr->sum_val = ptr->val = ptr->max_val = ptr->min_val = 0;
    ptr->sum = ptr->rev = 0;
    return ptr;
}

void split(node *v, const fkey_t &pk, node *&left, node *&right) {
    if (v == NULL) {
        left = right = NULL;
        return;
    }
    v->recalc();
    if (v->pk <= pk) {
        split(v->right, pk, v->right, right);
        left = v;	
    } else {
        split(v->left, pk, left, v->left);
        right = v;
    }
    v->recalc();
}

// Not intended to be used with delayed operations.
// Probably you just need to add v->recalc().
node* add(node *&v, const fkey_t &pk, const skey_t &sk) { 
    if (v == NULL || v->sk >= sk) {                        
        node* cur = new_node(pk, sk);
        split(v, pk, cur->left, cur->right);
        cur->recalc();
        return v = cur;
    }
    node* ptr = (pk <= v->pk) ? add(v->left, pk, sk) : add(v->right, pk, sk);
    v->recalc();
    return ptr;
}

// You MUST manually call left->recalc() and right->recalc()
// while calling this function !!!
node* merge(node *left, node *right) { 
    if (left == NULL) return right;     
    if (right == NULL) return left;
    if (left->sk <= right->sk) {
        if (left->right != NULL) left->right->recalc();
        left->right = merge(left->right, right);
        left->recalc();
        return left;
    } else {
        if (right->left != NULL) right->left->recalc();
        right->left = merge(left, right->left);
        right->recalc();
        return right;
    }
}

void erase(node *&v, const fkey_t &pk) {
    if (v == NULL) return;
    v->recalc();
    if (v->pk == pk) {
        node* tmp = merge(v->left, v->right);
        delete v;
        v = tmp;
        return;
    }
    if (pk <= v->pk) {
        erase(v->left, pk);
        v->recalc();
    } else {
        erase(v->right, pk);
        v->recalc();
    }
}

node* search(node *v, const fkey_t &pk) {
    if (v == NULL) return NULL;
    v->recalc();
    if (v->pk == pk) return v;
    return (pk <= v->pk) ? search(v->left, pk) : search(v->right, pk);
}

void clear(node *&v) {
    if (v == NULL) return;
    clear(v->left);
    clear(v->right);
    delete v;
    v = NULL;
}

node* begin() {
    if (root == NULL) return NULL;
    node* v = root;
    for (v->recalc(); v->left != NULL; v->recalc()) v = v->left;
    return v;
}

int count_less(const fkey_t &pk) {
    int ans = 0;
    for (node* v = root; v != NULL;) {
        v->recalc();
        if (pk <= v->pk) v = v->left;
        else {
            ans += v->left->size() + 1;
            v = v->right;
        }
    }
    return ans;
}

node* get_kth(int k) { //zero based
    for (node* v = root; v != NULL ;) {
        v->recalc();
        if (v->left->size() == k) return v;
        else if (v->left->size() > k) v = v->left;
        else {
            k -= v->left->size() + 1;
            v = v->right;
        }
    }
    return NULL;
}

void cut_k(node *v, int k, node *&left, node *&right) { // cut k nodes from left
    if (v == NULL) {
        left = right = NULL;
        return;
    }
    v->recalc();
    if (v->left->size() >= k) {
        right = v;
        cut_k(v->left, k, left, right->left);
    } else {
        left = v;
        cut_k(v->right, k - (v->left->size() + 1), left->right, right);
    }
    v->recalc();
}
