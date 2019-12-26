#include <cstdio>
#include <cstdlib>

#include "kth.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll long long
#define MAXN 500005

#define Parent(i) ( ((i) - 1) >> 1 )
#define LChild(i) ( 1 + ((i) << 1) )
#define RChild(i) ( (1 + (i)) << 1 )

// aa = bb = cc = [1..n]
int aa[MAXN], bb[MAXN], cc[MAXN];

struct Node {
    int x, y, z;
    Node(): x(0), y(0), z(0) {}
    Node(int _x, int _y, int _z): x(_x), y(_y), z(_z) {}
};

inline int cmp(const Node& n1, const Node& n2) {
    return compare(aa[n1.x], bb[n1.y], cc[n1.z], aa[n2.x], bb[n2.y], cc[n2.z]);
}

struct Heap {
    int cur_size;
    int max_size;
    Node *data;

    Heap(int _size): cur_size(0), max_size(_size) {
        data = new Node[max_size];
    }

    void insert(const Node& m) {
        // insert into heap
        if (cur_size < max_size)
            data[cur_size++] = m;
        else
            data[max_size - 1] = m;
        percolateUp();
    }

    Node pop() {
        Node tmp = data[0];
        data[0] = data[--cur_size];
        percolateDown();
        return tmp;
    }

    void percolateUp() {
        int i = cur_size - 1;
        while (0 < i) {
            int j = Parent(i);
            if (cmp(data[j], data[i])) break;
            swap(i, j);
            i = j;
        }
    }

    void percolateDown() {
        int i = 0, j;
        while (i != (j = properParent(i))) {
            swap(i, j);
            i = j;
        }
    }

    inline void swap(int i, int j) {
        Node tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
    }

    int properParent(int i) {
        int lc = LChild(i);
        int rc = RChild(i);

        if (lc >= cur_size) return i;
        int i_lc = cmp(data[i], data[lc]); // if i < lc
        if (rc >= cur_size) {
            if (i_lc) return i;
            else return lc;
        }
        if (i_lc) { // data[i] < data[lc]
            int rc_i = cmp(data[rc], data[i]);
            if (rc_i) return rc; // rc < i
            else return i;
        } else { // data[lc] < data[i]
            int lc_rc = cmp(data[lc], data[rc]);
            if (lc_rc) return lc;
            else return rc;
        }
    }

    void print() {
        printf("----------Current heap: -----------\n");
        int level_len = 1;
        for (int i = 0; i < cur_size; i += level_len, level_len <<= 1) {
            for (int j = 0; j < level_len && j + i < cur_size; j++) {
                Node& m = data[i + j];
                // printf("(%d,%d,%d)-%d ", m.x, m.y, m.z, a[aa[m.x]]+b[bb[m.y]]+c[cc[m.z]]);
            }
            printf("\n");
        }
    }
};

void get_kth(int n, int k, int *x, int *y, int *z) {
    Heap heap(k);

    // sort each array
    for (int i = 0; i < n; i++) aa[i] = bb[i] = cc[i] = i + 1;
    qsort(aa, n, sizeof(int), [](const void* x, const void *y) {
        return compare(*(int*)y, 1, 1, *(int*)x, 1, 1);
    });
    qsort(bb, n, sizeof(int), [](const void* x, const void *y) {
        return compare(1, *(int*)y, 1, 1, *(int*)x, 1);
    });
    qsort(cc, n, sizeof(int), [](const void* x, const void *y) {
        return compare(1, 1, *(int*)y, 1, 1, *(int*)x);
    });

    // starting from (0,0,0), add adjacent nodes to the heap
    // in each iteration, the smallest item is popped from the heap,
    // which is the (i+1)-th smallest item of the whole population
    heap.insert(Node(0, 0, 0));
    for (int i = 0; i < k - 1; i++) { // pop the min-(k-1) items
        Node m = heap.pop();
        if (m.x + 1 < n) // always walk along x axis
            heap.insert(Node(m.x + 1, m.y, m.z));
        if (!m.x && m.y + 1 < n) { // if on y-plane, walk along y axis
            heap.insert(Node(m.x, m.y + 1, m.z));
            if (!m.y && m.z + 1 < n) // if on z-axis, go up z axis as well
                heap.insert(Node(m.x, m.y, m.z + 1));
        }
    }

    Node& m = heap.data[0];
    *x = aa[m.x];
    *y = bb[m.y];
    *z = cc[m.z];
}
