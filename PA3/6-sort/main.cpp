#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "sort.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll long long
#define MAXN 1000005

/*
global vars
*/
// the current element of each segment that is to be sorted
int k[4];
// order of the segment, s.t. a[k[cols[0]]] > a[k[cols[1]]] ...
int cols[4];
// the end point of each segment
int size[4];

int *a;
int cur_pos; // a[cur_pos] is to be assigned
int tmp[MAXN]; // a copy of a

/*
helper functions
*/

// sort x, y, z
void sort_3(int &x, int &y, int &z) {
    int max, min;
    compare(x, y, z, &max, &min);
    y = x ^ y ^ z ^ max ^ min;
    x = min;
    z = max;
}

inline void swap(int &a, int &b) {
    int t = a;
    a = b, b = t;
}

// sort tmp[lo..lo+4]
void sort_4(int lo) {
    int tmp[4];
    for (int i = 0; i < 4; i++) tmp[i] = a[lo + i];
    sort_3(tmp[0], tmp[1], tmp[2]);
    int max, min;
    compare(tmp[0], tmp[1], tmp[3], &max, &min);
    if (tmp[3] == min) { // 3 < 0 < 1 < 2
        a[lo] = tmp[3];
        for (int i = 1; i < 4; i++) a[lo + i] = tmp[i - 1];
    } else if (tmp[3] == max) { // 0 < 1 < 2/3
        a[lo] = tmp[0];
        compare(tmp[1], tmp[2], tmp[3], &max, &min);
        a[lo + 2] = tmp[1] ^ tmp[2] ^ tmp[3] ^ max ^ min;
        a[lo + 1] = min;
        a[lo + 3] = max;
    } else { // 0 < 3 < 1 < 2
        a[lo] = tmp[0];
        a[lo + 1] = tmp[3];
        a[lo + 2] = tmp[1];
        a[lo + 3] = tmp[2];
    }
}

// sort tmp[k[cols[0..3/exclude]]]
int basic_sort_3(int exclude) {
    int x[3];
    int j = 0;
    int cols_cpy[4]; // the original order of the columns
    int left[3]; // the columns left to be sorted
    for (int i = 0; i < 4; i++) {
        cols_cpy[i] = cols[i];
        if (i == exclude) continue;
        x[j] = k[cols[i]];
        left[j++] = i;
    }

    int min, max;
    compare(tmp[x[0]], tmp[x[1]], tmp[x[2]], &max, &min);
    // rearrange columns
    for (int i = 0; i < 4; i++) {
        if (i == exclude) continue;
        if (tmp[k[cols_cpy[i]]] == min) {
            cols[left[0]] = cols_cpy[i];
        } else if (tmp[k[cols_cpy[i]]] == max) {
            cols[left[2]] = cols_cpy[i];
        } else { // mid
            cols[left[1]] = cols_cpy[i];
        }
    }
    return left[2]; // the larges column, which will not be compared in the next round
}

// sort tmp[k[cols[2..3]]]
void basic_sort_2() {
    int max, min;
    compare(tmp[k[cols[2]]], tmp[k[cols[3]]], tmp[k[cols[3]]], &max, &min);
    if (max == tmp[k[cols[2]]])
        swap(cols[2], cols[3]);
}

// 2-way merge
// merge columns c1 and c2
void merge_2(int c1, int c2) {
    auto get_var = [&]() {
        int col = cols[2];
        if (k[col] >= size[col] - 1) {
            // if one segment is empty, copy the other column over
            int other_col = cols[3];
            for (int i = k[other_col]; i < size[other_col]; i++) {
                a[cur_pos++] = tmp[i];
            }
            return false;
        }
        else {
            k[col]++;
            return true;
        }
    };

    do {
        basic_sort_2();
        a[cur_pos++] = tmp[k[cols[2]]];
    } while (get_var());
}

// 3-way merge
// merge 3 columns except 'except'
void merge_3(int except) {
    auto get_var = [&]() {
        int col = cols[1];
        if (k[col] >= size[col] - 1) {
            // if one segment is empty, go to 2-way merge
            merge_2(cols[2], cols[3]);
            return false;
        }
        else {
            k[col]++;
            return true;
        }
    };

    do {
        basic_sort_3(0);
        a[cur_pos++] = tmp[k[cols[1]]];
    } while (get_var());
}

// 4-way merge from lo to hi
void merge_sort(int lo, int hi) {
    // base
    if (hi - lo == 4) {
        sort_4(lo);
        return;
    } else if (hi - lo == 3) { // simple sort
        sort_3(a[lo], a[lo + 1], a[lo + 2]);
        return;
    } else if (hi - lo == 2) { // simple sort
        int max, min;
        compare(a[lo], a[lo], a[lo + 1], &max, &min);
        if (a[lo] == max) swap(a[lo], a[lo + 1]);
        return;
    } else if (hi - lo <= 1) {
        return;
    }

    // hi - lo > 4
    int segment_size = (hi - lo + 1) / 4;
    for (int i = 0; i < 3; i++) {
        merge_sort(lo + segment_size * i, lo + segment_size * (i + 1));
    }
    merge_sort(lo + segment_size * 3, hi);

    // make copy of area to be merged
    memcpy(tmp + lo, a + lo, sizeof(int) * (hi - lo));

    // init vars
    for (int i = 0; i < 4; i++) {
        k[i] = lo + segment_size * i;
        size[i] = k[i] + segment_size;
        cols[i] = i;
    }
    size[3] = hi;
    cur_pos = lo;

    auto get_var = [&]() {
        int col = cols[0];
        if (k[col] >= size[col] - 1) {
            // if one column is empty do 3-way merge
            merge_3(col);
            return false;
        }
        else {
            k[col]++;
            return true;
        }
    };
    // 4-way merge
    int skip = basic_sort_3(0); // the column to skip in the next round
    do {
        skip = basic_sort_3(skip);
        a[cur_pos++] = tmp[k[cols[0]]]; // append min to a
    } while (get_var());
}

void sort(int n, int limit, int *a) {
    ::a = a;
    merge_sort(0, n);
}