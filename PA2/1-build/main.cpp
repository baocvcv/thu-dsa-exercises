#include <cstdio>
#include <cstdlib>
#include <cassert>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

// #define _OJ_
struct TreeNode {
    int height, size;
    int down; // child
    int right; // right brother
    int left; // left brother
    int highest_brother; // highest brother after this node
    int parent; // parent of the node

    TreeNode():
        height(0), size(1),
        down(0), right(0), left(0), parent(0),
        highest_brother(0) {}
};

TreeNode *tree;
int M, N;

void print_tree(int c, int level) {
    if (c == 0) return;
    for (int i = 0 ; i < level * 2; i++) printf(" ");
    printf("%d: h=%d, s=%d, hb=%d, (%d %d)->%d ", c, tree[c].height, tree[c].size, tree[c].highest_brother, tree[c].left, tree[c].right, tree[c].parent);
    printf("\n");
    print_tree(tree[c].down, level + 1);
    print_tree(tree[c].right, level);
}

int get() { // select node
    int n;
    scanf("%d", &n);

    int cur = 1;
    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        int child = tree[cur].down;
        while (child != 0 && x > 0) {
            child = tree[child].right;
            x--;
        }
        if (child == 0) {
            while (++i < n) scanf("%d", &x);
            return cur;
        } else {
            cur = child;
        }
    }
    return cur;
}

bool update_height(int n) {
    if (n <= 0) return false;

    // update the height of node n
    int fc = tree[n].down;
    if (fc == 0) {
        tree[n].height = 0;
    } else if (tree[n].height != tree[fc].highest_brother + 1) {
        tree[n].height = tree[fc].highest_brother + 1;
    } else {
        return false; // height of n is unchanged
    }

    // update "highest_brother"
    while (n > 0) {
        if (tree[n].right > 0)
            tree[n].highest_brother = MAX(tree[n].height, tree[tree[n].right].highest_brother);
        else
            tree[n].highest_brother = tree[n].height;
        n = tree[n].left;
    }
    return true;
}

int remove() {
    int x = get(); // node to be removed

    // update size
    int par = tree[x].parent;
    while (par > 0) {
        tree[par].size -= tree[x].size;
        par = tree[par].parent;
    }

    // remove x from parent node
    int x_pred = tree[x].left; // as backup
    par = tree[x].parent;
    if (tree[par].down == x) { // x is p's first child
        tree[par].down = tree[x].right;
        if (tree[par].down > 0) // x has right brother
            tree[tree[par].down].left = 0;
    } else { // x has left brother
        tree[tree[x].left].right = tree[x].right;
        if (tree[x].right > 0) // x has right brother
            tree[tree[x].right].left = tree[x].left;
    }
    tree[x].highest_brother = tree[x].height;
    tree[x].parent = 0;
    tree[x].left = tree[x].right = 0;

    // update highest brothers
    int tmp = x_pred;
    while (tmp > 0) {
        if (tree[tmp].right > 0)
            tree[tmp].highest_brother = MAX(tree[tmp].height, tree[tree[tmp].right].highest_brother);
        else
            tree[tmp].highest_brother = tree[tmp].height;
        tmp = tree[tmp].left;
    }

    // update parent height
    while (update_height(par))
        par = tree[par].parent;

#ifndef _OJ_
    printf("after deletion\n");
    print_tree(1, 0);
    print_tree(x, 0);
#endif

    return x;
}

void insert(int x) {
    int par = get(); // the parent to insert into
    int rank;
    scanf("%d", &rank);

    // insert into parent
    int r_bro = tree[par].down; // insert between l and r
    int l_bro = 0;
    while (rank > 0 && r_bro > 0) {
        l_bro = r_bro;
        r_bro = tree[r_bro].right;
        rank--;
    }
    tree[x].parent = par;
    tree[x].right = r_bro;
    tree[x].left = l_bro;
    if (r_bro > 0) tree[r_bro].left = x;
    if (l_bro > 0) tree[l_bro].right = x;
    if (r_bro == tree[par].down) {
        tree[par].down = x;
    }

    // update size
    while (par > 0) {
        tree[par].size += tree[x].size;
        par = tree[par].parent;
    }

    // update brothers including x
    int tmp = x;
    while (tmp > 0) {
        if (tree[tmp].right > 0)
            tree[tmp].highest_brother = MAX(tree[tmp].height, tree[tree[tmp].right].highest_brother);
        else
            tree[tmp].highest_brother = tree[tmp].height;
        tmp = tree[tmp].left;
    }

    // update parent height
    par = tree[x].parent;
    while (update_height(par))
        par = tree[par].parent;
}

int *stack;
int stack_top;
int *stack2;
int stack2_top;
void push_brothers_onto_stack(int cur) {
    if (cur == 0) return;
    stack[stack_top++] = cur;
    push_brothers_onto_stack(tree[cur].right);
    stack2[stack2_top++] = cur;
}

int go_through_brothers(int cur) {
    TreeNode& n = tree[cur];
    if (n.right == 0) {
        n.highest_brother = n.height;
        return n.size;
    }
    int size = n.size + go_through_brothers(tree[cur].right);
    if (n.height < tree[n.right].highest_brother)
        n.highest_brother = tree[n.right].highest_brother;
    else
        n.highest_brother = n.height;
    return size;
}

void buildTree() {
    stack_top = stack2_top = 0;
    stack[stack_top++] = 1;
    stack2[stack2_top++] = 1;
    while (stack2_top > 0) {
        int cur = stack2[--stack2_top];
        push_brothers_onto_stack(tree[cur].down);
    } // all pushed onto stack

    while(stack_top > 0) { // update x from its children
        int x = stack[--stack_top];
        TreeNode& n = tree[x];
        if (n.down != 0) {
            n.size += go_through_brothers(n.down);
            n.height = tree[n.down].highest_brother + 1;
        }
    }
}

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
#endif
#ifdef _OJ_
    // setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    // setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
#endif

    scanf("%d %d", &N, &M);
    tree = new TreeNode[N+5];
    stack = new int[N + 1000];
    stack2 = new int[N + 1000];
    for (int i = 1; i <= N; i++) {
        int d;
        scanf("%d", &d);
        int x; // for reading input
        int cur;
        if (d > 0) {
            scanf("%d", &x);
            tree[x].parent = i;
            tree[i].down = x;
            cur = x;
        }
        for (int rank = 1; rank < d; rank++) {
            scanf("%d", &x);
            tree[x].parent = i;
            tree[cur].right = x;
            tree[x].left = cur;
            cur = x;
        }
    }
    tree[1].parent = 0;

    buildTree();

#ifndef _OJ_
    print_tree(1, 0); printf("\n");
#endif

    for (int i = 0; i < M; i++) {
        int cmd;
        scanf("%d", &cmd);
        if (cmd == 0) {
            int x = remove();
            insert(x);
#ifndef _OJ_
            print_tree(1, 0); printf("\n");
#endif
        } else {
            int x = get();
            if (cmd == 1)
                printf("%d\n", tree[x].height);
            else
                printf("%d\n", tree[x].size);
        }
    }
    return 0;
}
