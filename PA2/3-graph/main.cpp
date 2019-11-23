#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll long long
#define MAXN 100005

#define DEBUG
// #define _OJ_

// list
struct Edge {
    int v; // the other end of the edge
    int val; // val of the edge

    Edge (): v(-1), val(0) {}
    Edge (int _v):
        v(_v), val(0) {}
    Edge (int _v, int _val):
        v(_v), val(_val) {}
};

template <typename T>
struct Node {
    T data;
    Node<T> *next, *prev;
    Node (): next(NULL), prev(NULL) {}
    Node (T _t): data(_t), next(NULL), prev(NULL) {}
};

template <typename T>
struct List {
    Node<T> *head;
    Node<T> *tail;
    Node<T> *cur;

    List (): cur(NULL) {
        head = new Node<T>;
        tail = new Node<T>;
        head->next = tail;
        tail->prev = head;
    }

    void append(T data) {
        Node<T> *node = new Node<T>(data);
        node->next = tail; node->prev = tail->prev;
        tail->prev = node; node->prev->next = node;
    }
};

// graph nodes
struct GraphNode {
    int id;
    // int dtime; // time discovered, 0 = undiscovered
    int hca; // highest common ancestor
    int parent;
    // bool visited;
    List<Edge> edges;

    GraphNode () {}
    // GraphNode (int _id): id(_id), dtime(0), is_cut(false), visited(false) {}
    void connect_to(int v) { edges.append(v); }
};

GraphNode *graph; // start from 1
GraphNode *bcc_graph;
int n, m;

// stack
// int stack[100005];
// int stack_top;
bool visited[MAXN];
int dtime[MAXN];

void tarjan(int v_id, int& clock) { // look for bcc
    auto& v = graph[v_id];
    dtime[v_id] = v.hca = ++clock;
    // stack[stack_top++] = v.id; // push onto stack
    for (auto* e = v.edges.head->next; e != v.edges.tail; e = e->next) {
        auto &u = graph[e->data.v];
        if (!dtime[u.id]) { // u is not visited
            u.parent = v.id;
            tarjan(u.id, clock);
            if (u.hca < dtime[v_id]) {
                v.hca = MIN(v.hca, u.hca);
            } else { // found a bcc
            }
        } else if (dtime[u.id] && !visited[u.id]) { // u is discovered
            if (u.id != v.parent) v.hca = MIN(v.hca, dtime[u.id]);
        }
    }
    visited[v.id] = true;
}

int query(int u, int v, int* res) {
    int len = 0;
    int tmp = v;
    while (true) {
        GraphNode& n = graph[tmp];
        if (n.parent == u) {
            break;
        }

        if (n.hca >= dtime[n.parent]) {
            res[len++] = n.parent;
        }
        tmp = n.parent;
    }
    res[len++] = u;
    res[len++] = v;

    auto cmp = [](const void* i, const void* j) {
        return *(int*)i - *(int*)j;
    };
    qsort(res, len, sizeof(int), cmp);
    return len;
}

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

    scanf("%d %d", &n, &m);
    graph = new GraphNode[n+1];
    bcc_graph = new GraphNode[n+1];
    for (int i = 0; i <= n; i++) {
        graph[i].id = i;
    }
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        graph[u].connect_to(v);
        graph[v].connect_to(u);
    }

    // query
    int q;
    scanf("%d", &q);
    int res[MAXN];
    for (int i = 0; i < q; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        memset(visited, 0, sizeof(bool) * MAXN);
        memset(dtime, 0, sizeof(int) * MAXN);
        int clock = 0;
        tarjan(u, clock);
        int s = query(u, v, res);
        for(int j = 0; j < s; j++) {
            printf("%d ", res[j]);
        }
        printf("\n");
    }
    return 0;
}
