#include <cstdio>
#include <cstdlib>
#include <cassert>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll long long
#define MAXN 100005

// #define _OJ_

struct ListNode {
    int val;
    int size;
    ListNode* next;
    ListNode(int _v):
        val(_v), size(1), next(NULL) {}
};

struct List {
    ListNode* head;
    List() {
        head = new ListNode(-1);
    }
    void push(int v) {
        if (head->next == NULL) {
            head->next = new ListNode(v);
            return;
        }
        if (v > head->next->val) {
            ListNode* nn = new ListNode(v);
            nn->next = head->next;
            head->next = nn;
        } else {
            head->next->size++;
        }
    }

    void pop() {
        ListNode* c = head->next;
        assert(c != NULL);
        c->size--;
        if (c->size == 0) {
            head->next = c->next;
            delete c;
        }
    }

    int max() {
        if (head->next == NULL)
            return -1;
        return head->next->val;
    }

    int size() {
        int s = 0;
        ListNode* tmp = head->next;
        while (tmp != NULL) {
            s++;
            tmp = tmp->next;
        }
        return s;
    }
};

struct TreeNode {
    int height, size;
    int down;
    int right;

    // List queap;
    int highest_brother; // highest brother after this node

    TreeNode():
        height(0), size(1),
        down(0), right(0) {}
};

TreeNode *tree;
// TreeNode tree[20];
int M, N;

int *stack;
int stack_top;

int *route;
int route_len;
int prev;
int get() { // select node
    int n;
    scanf("%d", &n);

    route_len = 0;
    int cur = 1;
    route[route_len++] = cur;
    int i;
    for (i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        int child = tree[cur].down;
        while (child != 0 && x > 0) {
            prev = child;
            child = tree[child].right;
            x--;
        }
        if (child == 0) {
            while (++i < n) scanf("%d", &x);
            return cur;
        } else {
            cur = child;
            route[route_len++] = cur;
        }
    }
    return cur;
}

bool update_height(int parent, int child) {
    // update parent height when its child changes

}

int remove() {
    int x = get(); // node to be removed
    route_len--;

    TreeNode* p = &tree[route[--route_len]]; // parent of x
    // update size
    p->size -= tree[x].size;
    // update queap
    stack_top = 0; // init stack
    int tmp = p->down;
    // for (int i = 0; i <= tree[x].rank; i++) {
    while (tmp != x) {
        p->queap.pop();
        stack[stack_top++] = tree[tmp].height;
        tmp = tree[tmp].right;
    }
    p->queap.pop();
    // stack_top--; // the last element must be x
    while (--stack_top >= 0) {
        p->queap.push(stack[stack_top]);
    }
    // remove from parent node
    if (p->down == x) { // x is p's first child
        p->down = tree[x].right;
    } else {
        tree[prev].right = tree[x].right;
    }
    // update height if need to and set flag
    bool flag = false;
    if (p->height != p->queap.max() + 1) {
        p->height = p->queap.max() + 1;
        flag = true;
    }
    // if need to update, go up the route
    TreeNode* p_last = p;
    int x_last = route[route_len];
    while (route_len > 0) {
        TreeNode* p_cur = &tree[route[--route_len]];
        p_cur->size -= tree[x].size;
        if (flag) { // update queap
            stack_top = 0;
            tmp = p_cur->down;
            while (tmp != x_last){
                p_cur->queap.pop();
                stack[stack_top++] = tree[tmp].height;
                tmp = tree[tmp].right;
            }
            p_cur->queap.pop();
            stack[stack_top++] = tree[tmp].height;
            tmp = tree[tmp].right;
            while (--stack_top >= 0) // push everything back again
                p_cur->queap.push(stack[stack_top]);
            if (p_cur->height != p_cur->queap.max() + 1)
                p_cur->height = p_cur->queap.max() + 1;
            else
                flag = false;
        }
        p_last = p_cur;
        x_last = route[route_len];
    }
    return x;
}

void insert(int x) {
    int parent = get(); // parent to insert into
    route_len--;
    int rank;
    scanf("%d", &rank);

    if (x == 1) return;

    TreeNode* p = &tree[parent];
    TreeNode* tx = &tree[x];
    // update size
    p->size += tx->size;
    // find the one before and after x
    stack_top = 0;
    int pred = -1;
    int succ = p->down;
    for (int i = 0; i < rank; i++) {
        stack[stack_top++] = succ;
        pred = succ;
        succ = tree[succ].right;
    }
    // insert to parent node
    if (rank == 0)
        p->down = x;
    else
        tree[pred].right = x;
    tx->right = succ;
    int tmp = p->down;
    stack_top = 0;
    while (tmp != x) {
        p->queap.pop(); // pop "rank" number of elements
        stack[stack_top++] = tree[tmp].height;
        tmp = tree[tmp].right;
    }
    // update queap
    p->queap.push(tx->height);
    while (--stack_top >= 0)
        p->queap.push(stack[stack_top]);
    // update height
    bool flag = false;
    if (p->height != p->queap.max() + 1) {
        p->height = p->queap.max() + 1;
        flag = true;
    }
    // trasmit height
    TreeNode* p_last = p;
    int x_last = route[route_len];
    while (route_len > 0) {
        p = &tree[route[--route_len]];
        p->size += tree[x].size;
        if (flag) {
            stack_top = 0;
            int tmp = p->down;
            while (tmp != x_last) {
                p->queap.pop();
                stack[stack_top++] = tree[tmp].height;
                tmp = tree[tmp].right;
            }
            p->queap.pop();
            stack[stack_top++] = tree[tmp].height;
            tmp = tree[tmp].right;
            while (--stack_top >= 0)
                p->queap.push(stack[stack_top]);
            if (p->height != p->queap.max() + 1)
                p->height = p->queap.max() + 1;
            else
                flag = false;
        }
        p_last = p;
        x_last = route[route_len];
    }
}

int *stack2;
int stack2_top;

void push_brothers_onto_stack(int cur) {
    if (cur == 0) return;
    stack[stack_top++] = cur;
    push_brothers_onto_stack(tree[cur].right);
    stack2[stack2_top++] = cur;
}

int go_through_brothers(int cur, List& queap) {
    TreeNode& n = tree[cur];
    if (n.right == 0) {
        // queap.head->next = new ListNode(n.height);
        queap.push(n.height);
        return n.size;
    }

    int size = n.size + go_through_brothers(tree[cur].right, queap);

    if (n.height > queap.head->next->val) {
        queap.push(n.height);
    } else {
        queap.head->next->size++;
    }
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
            n.size += go_through_brothers(n.down, n.queap);
            n.height = n.queap.head->next->val + 1;
        }
    }
}

void print_tree(int c, int level) {
    if (c == 0) return;
    for (int i = 0 ; i < level * 2; i++) printf(" ");
    printf("%d: h=%d, s=%d, qp: ", c, tree[c].height, tree[c].size);
    // for (auto p = tree[c].queap.head->next; p != NULL; p = p->next)
    //     printf("%d*%d- ", p->val, p->size);
    printf("\n");
    print_tree(tree[c].down, level + 1);
    print_tree(tree[c].right, level);
}



int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
#ifdef _OJ_
    // setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    // setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
#endif

    scanf("%d %d", &N, &M);
    tree = new TreeNode[N+5];
    stack = new int[N+10];
    stack2 = new int[N+10];
    route = new int[N+10];
    for (int i = 1; i <= N; i++) {
        int d;
        scanf("%d", &d);
        int x; // for reading input
        int cur;
        if (d > 0) {
            scanf("%d", &x);
            tree[i].down = x;
            cur = x;
        }
        for (int rank = 1; rank < d; rank++) {
            scanf("%d", &x);
            tree[cur].right = x;
            cur = x;
        }
    }

    buildTree();

#ifndef _OJ_
    print_tree(1, 0); printf("\n");
#endif

    for (int i = 0; i < M; i++) {
        int cmd;
        scanf("%d", &cmd);
        if (cmd == 0) {
            // remove();
            insert(remove());
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
