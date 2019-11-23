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
    int dtime; // time discovered, 0 = undiscovered
    int hca; // highest common ancestor
    int parent;
    bool visited;
    List<Edge> edges;

    List<Edge> bcc_edges; // used in bcc graph
    bool is_cut; // whether the nodes is an articulation node on the graph
    int connected_to;

    GraphNode (): dtime(0), is_cut(false), visited(false) {}
    GraphNode (int _id): id(_id), dtime(0), is_cut(false), visited(false) {}
    void connect_to(int v) { edges.append(v); }
};

GraphNode *graph; // start from 1
GraphNode *bcc_graph;
int n, m;

// stack
int stack[MAXN];
int stack_top;

void tarjan(int v_id, int& clock) { // look for bcc
    auto& v = graph[v_id];
    v.dtime = v.hca = ++clock;
    stack[stack_top++] = v.id; // push into stack
    for (auto* e = v.edges.head->next; e != v.edges.tail; e = e->next) {
        auto &u = graph[e->data.v];
        if (!u.dtime) { // u is not visited
            u.parent = v.id;
            tarjan(u.id, clock);
            if (u.hca < v.dtime) {
                v.hca = MIN(v.hca, u.hca);
            } else { // found a bcc
                // v is an articulation point
                v.is_cut = true;
                int x = stack[--stack_top]; // top of the stack
                int rep = x;
                // connnect v - rep
                graph[rep].bcc_edges.append(v.id);
                v.bcc_edges.append(rep);
                if (!graph[rep].is_cut) graph[rep].connected_to = rep;
                while (u.id != x) {
                    // everything popped is in the bcc
                    x = stack[--stack_top];
                    GraphNode& n = graph[x];
                    if (n.is_cut) {
                        n.bcc_edges.append(rep); // connect cut - rep
                        graph[rep].bcc_edges.append(n.id);
                    } else {
                        n.connected_to = rep; // connect n - rep
                    }
                }
            }
        } else if (u.dtime && !u.visited) { // u is discovered
            if (u.id != v.parent) v.hca = MIN(v.hca, u.dtime);
        }
    }
    v.visited = true;
}


int route[MAXN];
int route_len;
bool visited[MAXN];
bool dfs(int cur, int dst) {
    visited[cur] = true;
    route[route_len++] = cur;
    if (cur == dst) return true;

    GraphNode& n = graph[cur];
    for (auto* e = n.bcc_edges.head->next; e != n.bcc_edges.tail; e = e->next) {
        Edge u = e->data;
        if (!visited[u.v]) {
            if (dfs(u.v, dst)) {
                return true;
            }
        }
    }
    route_len--;
    return false;
}

int query(int u, int v, int* res) {
    // stores the result in res, returns the length of res
    int len = 0;
    res[len++] = u;
    res[len++] = v;

    if (!graph[u].is_cut) u = graph[u].connected_to;
    if (!graph[v].is_cut) v = graph[v].connected_to;

    memset(visited, 0, sizeof(bool) * MAXN);
    route_len = 0;
    dfs(u, v);

    // only keep the cut points
    for (int i = 1; i < route_len - 1; i++) {
        if (graph[route[i]].is_cut) {
            res[len++] = route[i];
        }
    }
    auto cmp = [](const void* i, const void* j) {
        return *(int*)i - *(int*)j;
    };
    qsort(res, len, sizeof(int), cmp);
    return len;
}

int main() {
// #ifndef _OJ_
//     freopen("input.txt", "r", stdin);
//     freopen("output.txt", "w", stdout);
// #endif
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
    // tarjan
    int clock = 0;
    for (int i = 1; i <= n; i++) {
        if (!graph[i].visited) { // dtime == 0
            tarjan(i, clock);
        }
    }
    // query
    int q;
    scanf("%d", &q);
    int res[MAXN];
    for (int i = 0; i < q; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        if (u == v) printf("%d\n", u);
        else {
            int s = query(u, v, res);
            for(int j = 0; j < s; j++) {
                printf("%d ", res[j]);
            }
            printf("\n");
        }
    }
    return 0;
}
