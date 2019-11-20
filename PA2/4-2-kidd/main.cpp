#include <cstdio>
#include <cstdlib>

using namespace std;

struct Node {
    unsigned int l_bound, r_bound; // [l_bound, r_bound)
    long long tag; // num of times turned
    Node *l_child, *r_child;
};

Node *root;
char cmd_type[200002];
unsigned int input[200002][2];
unsigned int boundaries[400001];
int m;
int no_of_points;

Node* build_tree(int l, int r) {
    if (l >= r) { // should not build new node
        return NULL;
    }

    Node *p = new Node;
    p->l_bound = boundaries[l];
    p->r_bound = boundaries[r];
    p->tag = 0;
    if (r - l == 1) {
        p->l_child = NULL;
        p->r_child = NULL;
    } else { // r - l > 1
        int mid = (l + r) >> 1;
        p->l_child = build_tree(l, mid);
        p->r_child = build_tree(mid, r);
    }
    return p;
}

/* turn the cards in [l, r], strating at tree node c */
void turn(unsigned int l_b, unsigned int r_b, Node *c) {
    if (c == NULL) return; // base

    if (l_b <= c->l_bound && c->r_bound - 1 <= r_b) { // inside
        c->tag++; // update tag
    } else if (c->r_bound <= l_b || c->l_bound > r_b) { // outside
        return; // do nothing
    } else { // intersects
        turn(l_b, r_b, c->l_child);
        turn(l_b, r_b, c->r_child);
    }
}

/* query card at pos k */
long long query(unsigned int l_b, unsigned int r_b, Node *c) {
    if (c == NULL) return 0; // base

    long long sum = 0;
    if (l_b <= c->l_bound && c->r_bound - 1 <= r_b) { // inside
        sum += c->tag * (c->r_bound - c->l_bound);
        sum += query(l_b, r_b, c->l_child);
        sum += query(l_b, r_b, c->r_child);
    } else if (c->r_bound <= l_b || c->l_bound > r_b) { // outside
        // do nothing
    } else {
        sum += query(l_b, r_b, c->l_child);
        sum += query(l_b, r_b, c->r_child);
    }
    // printf("+[%d,%d).%d", c->l_bound, c->r_bound, sum);
    return sum;
}

void print_tree(Node *c, int level) {
    if (c == NULL) return;
    for (int i = 0 ; i < level * 2; i++) printf("-");
    printf("[%d, %d) - %lld\n", c->l_bound, c->r_bound, c->tag);
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

    unsigned int n;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; i++) {
        char s[3];
        scanf("%s", s);
        cmd_type[i] = s[0];
        scanf("%d %d", &input[i][0], &input[i][1]);
        boundaries[i*2] = input[i][0];
        boundaries[i*2+1] = input[i][1] + 1; // so that the segments are [a, b)
    }
    // sort and deduplicate
    auto cmp = [](const void *a, const void *b){
        return int(*(unsigned int*)a - *(unsigned int*)b);
    };
    qsort(boundaries, m*2, sizeof(unsigned int), cmp);
    int i = 0;
    for (int j = 1; j < 2 * m; j++) {
        if (boundaries[i] != boundaries[j]) {
            boundaries[++i] = boundaries[j];
        }
    }
    no_of_points = i + 1;

    root = build_tree(0, no_of_points - 1);
    // print_tree(root, 0);
    for (int i = 0; i < m; i++) {
        if (cmd_type[i] == 'H') {
            turn(input[i][0], input[i][1], root);
            // print_tree(root, 0);
        } else {
            printf("%lld\n", query(input[i][0], input[i][1], root));
        }
    }
    return 0;
}
