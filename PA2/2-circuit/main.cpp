#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll unsigned long long

// #define DEBUG
#define _OJ_

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
        // -1 means same
        // at most 63
        int i = size - 1;
        for(; i >= 0; i--) {
            if (((prefix >> i) & 1) != ((key >> i) & 1)) {
                return i;
            }
        }
        return i;
    }

    void split(int pos, ll suffix, int suf_len, int _id) {
        Node *n1 = new Node(suffix, suf_len, _id); // from suffix
        n1->p = this;
        Node *n2 = new Node(key, pos, id); // split from current
        n2->l_c = l_c; n2->r_c = r_c; n2->p = this;
        if (n2->l_c != NULL) n2->l_c->p = n2;
        if (n2->r_c != NULL) n2->r_c->p = n2;

        if ((key >> pos) & 1) {
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
        // delete a child and merge with the other
        Node* p; // the node to be merged
        if (c == l_c) { // should delete left child
            p = r_c;
            size++;
            key <<= 1;
            key |= 1;
        } else { // should delete right child
            p = l_c;
            size++;
            key <<= 1;
        }
        l_c = p->l_c; r_c = p->r_c;
        if (l_c != NULL) l_c->p = this;
        if (r_c != NULL) r_c->p = this;
        size += p->size;

        ll mask = 1;
        mask = (mask << p->size) - 1;
        p->key &= mask;
        key <<= p->size;
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
    while(pos >= 0) {
        if (p->size == 0) { // is not compressed
            if (p->l_c == NULL && p->r_c == NULL){ // is leaf
                p->id = id;
                return;
            } else { // not leaf
                if ((c >> pos) & 1) p = p->r_c; // c[pos] == 1
                else p = p->l_c;
                pos--;
            }
        } else { // is compressed
            int res = p->cmp(c >> (pos - p->size + 1));

            if (res >= 0) {
                // split at the point
                int suf_len = pos + 1 - p->size + res;
                p->split(res, c, suf_len, id);
                return;
            } else { // match completed
                pos -= p->size;
                if (pos < 0) { // same
                    p->id = id;
                    return;
                }
                if ((c >> pos) & 1) p = p->r_c; // c[pos] == 1
                else p = p->l_c;
                pos--;
                if (pos < 0) { // same
                    p->id = id;
                    return;
                }
            }
        }
    }
    // printf("****should not reach this****** add\n");
}

void delete_from_tree(ll c, int id) {
    // find the node
    // if id matches, delete, merge brother with dad
    Node *p = root;
    int pos = 63;
    while(pos >= 0) {
        pos -= p->size;
        if (p->l_c == NULL && p->r_c == NULL) { // is leaf
            #ifdef DEBUG
            printf("%d %d %d: %016llx\n", p->id, id, pos, c);
            #endif
            if (p->id == id) {
                p->p->delete_child(p);
            }
            return;
        } else {
            if ((c >> pos) & 1) p = p->r_c;
            else p = p->l_c;
            pos--;
        }
    }
    p->p->delete_child(p);
}

int query(ll c) {
    // query the tree and return the id
    Node *p = root;
    int pos = 63;
    while (pos >= 0) {
        pos -= p->size; // move through current node
        if (p->l_c == NULL && p->r_c == NULL) { // is leaf
            return p->id;
        } else { // not leaf
            if ((c >> pos) & 1) p = p->l_c; // opposite direction
            else p = p->r_c;
            pos--;
        }
    }
    return p->id;
}

void print_tree(Node *c, int level) {
    if (c == NULL) return;
    for (int i = 0 ; i < level * 2; i++) printf(".");
    if (c->id >= 0) printf("%016llx(%d) - %d\n", c->key, c->size, c->id);
    else  printf("%016llx(%d)\n", c->key, c->size);
    print_tree(c->l_c, level + 1);
    print_tree(c->r_c, level + 1);
}

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    // setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    // setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

    int n, k;
    scanf("%d %d", &n, &k);
    k = MIN(k, n - 2);

    ll *data = new ll[n+2];
    char s[70];
    for (int i = 0; i < n; i++) {
        scanf("%s", s);
        data[i] = 0;
        for (int j = 0; j < 64; j++) {
            data[i] = data[i] * 2 + s[j] - '0';
        }
    }

    int size = 2 * k + 3;
    int start, end; // current working window
    start = end = n;
    int *res = new int[n];
    int query_id;
    for (start = n - 1; start >= 0; start--) {
        if (end - start - 1 >= size) {
            end--;
            delete_from_tree(data[end], end);
            #ifdef DEBUG
            printf("[Deleted] %llx: %d\n", data[end], end);
            print_tree(root, 0);
            #endif
        }

        add_to_tree(data[start], start);
        #ifdef DEBUG
        printf("[Added]%d: %llx\n", start, data[start]);
        print_tree(root, 0);
        #endif

        // start query
        if (start <= n - k - 2) {
            query_id = start + k + 1; // the id to be queried
            #ifdef DEBUG
            printf("[query] %016llx: %d\n", data[query_id], query(data[query_id]));
            #endif
            res[query_id] = query(data[query_id]);
            query_id--;
        }
    }

    while (query_id >= 0) {
        if (end - query_id - 1 > k + 1) {
            end--;
            delete_from_tree(data[end], end);
        }

        #ifdef DEBUG
        printf("[Deleted] %llx: %d\n", data[end], end);
        print_tree(root, 0);
        #endif

        #ifdef DEBUG
        printf("[query] %016llx: %d\n", data[query_id], query(data[query_id]));
        #endif
        res[query_id] = query(data[query_id]);
        query_id--;
    }

    for (int i = 0; i < n; i++) {
        printf("%d\n", res[i]);
    }

    return 0;
}
