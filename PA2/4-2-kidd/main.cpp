#include <cstdio>
#include <cstdlib>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

struct Node {
    long long l_bound, r_bound; // [l_bound, r_bound)
    long long len;
    long long tag; // num of times turned
    long long sum; // the sum of all the sub-nodes
    Node *l_child, *r_child;

    Node (long long _l, long long _r):
        l_bound(_l), r_bound(_r), len(_r - _l),
        tag(0), sum(0),
        l_child(NULL), r_child(NULL) {}
};

Node *root;
char cmd_type[300002];
long long input[300002][2];
long long boundaries[600001];
long long m;
long long no_of_points;

/* build tree recursively from query points */
Node* build_tree(long long l, long long r) {
    if (l >= r) { // should not build new node
        return NULL;
    }
    Node *p = new Node(boundaries[l], boundaries[r]);
    if (r - l > 1) {
        long long mid = (l + r) >> 1;
        p->l_child = build_tree(l, mid);
        p->r_child = build_tree(mid, r);
    }
    return p;
}

/* turn the cards in [l, r), strating at tree node c */
long long turn(long long l_b, long long r_b, Node *c) { // returns how much is increased
    if (c == NULL) return 0; // base

    if (l_b <= c->l_bound && c->r_bound <= r_b) { // inside
        c->tag++; // update tag
        return c->len;
    } else if (c->r_bound <= l_b || r_b <= c->l_bound) { // outside
        return 0; // do nothing
    } else { // intersects
        if (c->l_child == NULL && c->r_child == NULL){ // is leaf node
            long long l = MAX(c->l_bound, l_b);
            long long r = MIN(c->r_bound, r_b);
            c->sum += r - l;
            return r - l;
        } else {
            long long delta = turn(l_b, r_b, c->l_child);
            delta += turn(l_b, r_b, c->r_child);
            c->sum += delta;
            return delta;
        }
    }
}

/* query card in [l_b, r_b) */
long long query(long long l_b, long long r_b, Node *c) {
    if (c == NULL) return 0; // base

    long long sum = 0;
    if (l_b <= c->l_bound && c->r_bound <= r_b) { // inside
        sum += c->tag * c->len + c->sum;
    } else if (c->r_bound <= l_b || r_b <= c->l_bound) { // outside
        // do nothing
    } else { // intersect
        long long r = MIN(c->r_bound, r_b);
        long long l = MAX(c->l_bound, l_b);
        sum += c->tag * (r - l);
        sum += query(l_b, r_b, c->l_child);
        sum += query(l_b, r_b, c->r_child);
    }
    return sum;
}

void print_tree(Node *c, int level) {
    if (c == NULL) return;
    for (int i = 0 ; i < level * 2; i++) printf("-");
    printf("[%d, %d) - %lld, %lld\n", c->l_bound, c->r_bound, c->tag, c->sum);
    print_tree(c->l_child, level + 1);
    print_tree(c->r_child, level + 1);
}

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

    long long n;
    scanf("%lld %lld", &n, &m);
    long long nn = 0;
    for (long long i = 0; i < m; i++) {
        char s[5];
        scanf("%s", s);
        cmd_type[i] = s[0];

        scanf("%lld %lld", &input[i][0], &input[i][1]);
        input[i][1]++; // so that the segments are [a, b)

        if (s[0] == 'Q') {
            boundaries[nn++] = input[i][0];
            boundaries[nn++] = input[i][1];
        }
    }
    // sort and deduplicate
    auto cmp = [](const void *a, const void *b){
        return int(*(long long*)a - *(long long*)b);
    };
    qsort(boundaries, nn, sizeof(long long), cmp);
    long long i = 0;
    for (long long j = 1; j < nn; j++) {
        if (boundaries[i] != boundaries[j]) {
            boundaries[++i] = boundaries[j];
        }
    }
    no_of_points = i + 1;

    root = build_tree(0, no_of_points - 1);
    for (long long i = 0; i < m; i++) {
        if (cmd_type[i] == 'H') {
            turn(input[i][0], input[i][1], root);
        } else {
            printf("%lld\n", query(input[i][0], input[i][1], root));
        }
    }
    return 0;
}
