#include <cstdio>
#include <cstdlib>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll unsigned long long

struct Node {
    ll key;
    int size; // number of significant bits of the key, 1 <= size <= 64
    int id; // if 0 <= id < n, is leaf node; otherwise, id < 0
    Node *l_c, *r_c;
    Node *p;

    Node (ll _key, int _s, int _id):
        key(_key), size(_s), id(_id),
        l_c(NULL), r_c(NULL) {}

    int cmp(ll prefix) {
        // return the first pos where prefix differs from key
        // 0 means same
        // at most 64
        int i = size - 1;
        for(; i >= 0; i--) {
            ll mask = (1 << i);
            if ((prefix & mask) != (key & mask)) {
                return i;
            }
        }
        return i;
    }

    void split(int pos, ll suffix, int suffix_len, int _id) {
        Node *n1 = new Node(suffix, suffix_len, _id); // from suffix
        n1->p = this;
        Node *n2 = new Node(key, pos, id); // split from current
        n2->l_c = l_c; n2->r_c = r_c; n2->p = this;

        ll mask = (1 << pos);
        if (key & mask) {
            l_c = n1;
            r_c = n2;
        } else {
            l_c = n2;
            r_c = n1;
        }

        // modify current node
        id = -1; // not a leaf node any more
        size -= pos + 1;
        key >>= pos + 1;
    }

    void delete_child(Node *c) {
        // delete a chile and merge with the other
        Node* p = l_c; // the node to be merged
        if (c == p) p = r_c;
        l_c = p->l_c; r_c = p->r_c;
        size += p->size;

        ll mask = (1 << p->size) - 1;
        p->key &= mask;
        key << p->size;
        key |= p->key;
        id = p->id;

        if (l_c != NULL) l_c->p = this;
        if (r_c != NULL) r_c->p = this;

        delete p;
        delete c;
    }
};

Node *root;

void add_to_tree(ll c, int id) {
    // update the tree with component c
    // if empty, simply add
    if (root == NULL) {
        root = new Node(c, 64, id);
        root->p = root;
        return;
    }

    // go along the tree, find first mismatch
    // create new node with matched part
    // creat new node with the rest of c
    Node *p = root;
    int pos = 63;
    while(true && p != NULL) {
        if (p->size == 0) { // is not compressed
            if (p->l_c == NULL && p->r_c == NULL){ // is leaf
                p->id = id;
                break;
            } else { // not leaf
                ll mask = 1 << pos;
                if (c & mask) p = p->r_c; // c[pos] == 1
                else p = p->l_c;
                pos--;
            }
        } else { // is compressed
            ll mask = ((1 << p->size) - 1) << (pos - size);
            ll prefix = (c & mask) >> (pos - size);
            int res = p->cmp(prefix);

            if (res >= 0) {
                // split at the point
                p->split(res, c, pos, id);
                break;
            } else {
                pos -= p->size;
            }
        }
    }
}

void delete_from_tree(ll c, int id) {
    // find the node
    // if id matches, delete, merge brother with dad
    Node *p = root;
    int pos = 63;
    while(pos >= 0) {
        if (p->size == 0) {
            if (p->l_c == NULL && p->r_c == NULL) { // is leaf
                if (p->id == id) {
                    p->p->delete_child(p);
                    break;
                }
            } else {
                ll mask = 1 << pos;
                if (c & mask) p = p->r_c;
                else p = p->l_c;
                pos--;
            }
        } else { // must match
            pos -= p->size;
        }
    }
    if (pos < 0) { // leaf node is compressed
        p->p->delete_child(p);
    }
}

int query(ll c) {
    // query the tree and return the id
    Node *p = root;
    int pos = 63;
    while (pos >= 0) {
        if (p->size == 0) {
            if (p->l_c == NULL && p->r_c == NULL) { // is leaf
                return p->id;
            } else {
                ll mask = 1 << pos;
                if (c & mask) p = p->l_c; // opposite direction
                else p = p->r_c;
                pos--;
            }
        } else { // have nowhere to go
            pos -= p->size;
        }
    }
    if (pos < 0) {
        return p->id;
    }
}

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

    int n, k;
    scanf("%d %d", &n, &k);

    ll *data = new ll[n+2];
    for (int i = 0; i < n; i++) {
        scanf("%lld", data + i);
    }

    int size = 2 * k + 3;
    int start, end; // current working window
    start = end = n;
    for (start = n - 1; start >= 0; start--) {
        add_to_tree(data[start], start);
        // query
        if (start <= n - k - 2) {
            int query_id = start + k + 1; // the id to be queried
            printf("%d\n", query(data[query_id]));
        }
        if (end - start > size) {
            end--;
            delete_from_tree(data[end], end);
        }
    }

    while (end > k + 1) {
        int query_id = end - k - 2; // the id to be queried
        printf("%d\n", query(data[query_id]));

        end--;
        delete_from_tree(data[end], end);
    }
    return 0;
}
