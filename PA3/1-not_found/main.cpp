#include <cstdio>
#include <cstring>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll long long
#define MAXN 1 << 24

const unsigned int bytes[] = {
    0x80,
    0x80 >> 1,
    0x80 >> 2,
    0x80 >> 3,
    0x80 >> 4,
    0x80 >> 5,
    0x80 >> 6,
    0x80 >> 7,
};

// use bitmap to store a dictionary tree
// edge represents chars, going left = '0', going right = '1'
// node(=0/1) represents if the path has been taken
struct BitMapTree {
    int total_len;
    char* data;

    BitMapTree (int _len) {
        total_len = (_len + 7) >> 3;
        data = new char[total_len];
        reset();
        set_bit(0); // set root node to 1
    }
    // basic bitmat operations
    inline void reset() { memset(data, 0, sizeof(char) * total_len); }
    inline void set_bit(int k) { data[k >> 3] |= bytes[k & 0x07]; }
    inline int check(int k) { return (data[k >> 3] & bytes[k & 0x07]) != 0; }

    // insert string s into tree
    // e.g. if the string to be inserted is s = 1001001,
    // then (s-1) is the bit of the lowest node corresponding to s
    inline void insert_string(unsigned int s) {
        while (s > 0) {
            if (check(s - 1)) {
                break;
            } else {
                set_bit(s - 1);
                s >>= 1;
            }
        }
    }

    // res is used to store the result, in reverse order
    int get_result(char* res) {
        // find the first unoccupied node
        int pos;
        for (int i = 0; i < total_len; i++) {
            if (data[i] != '\377') { // contains 0
                for (int j = 0; j < 8; j++) {
                    if ((data[i] & bytes[j]) == 0) { // find the first 0
                        pos = (i << 3) + j;
                        break;
                    }
                }
                break;
            }
        }

        // walk up the tree and save the result
        int len = 0;
        for (int i = pos; i > 0; i = (i - 1) / 2) {
            // if i % 2 == 0, current node is right child, so set result to '1'
            // & vice versa
            if (i & 1)
                res[len++] = '0';
            else
                res[len++] = '1';
        }

        return len;
    }

    void print_line(int end) {
        printf("1\n");
        int level_len = 1;
        for (int i = 1; i < end; i += level_len) {
            level_len <<= 1;
            for (int j = 0; j < level_len; j+= 2) {
                printf("%d%d ", check(i+j), check(i+j+1));
            }
            printf("\n");
        }
    }
};

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
#ifdef _OJ_
    setvbuf(stdin, new char[1 << 21], _IOFBF, 1 << 21);
    setvbuf(stdout, new char[1 << 6], _IOFBF, 1 << 6);
#endif

    BitMapTree tree(1 << 25);

    // read first 24 chars
    int len, start;
    unsigned int s = 0;
    char c = getchar();
    for(len = start = 0; len < 24 && c != '\n'; len++) {
        s <<= 1;
        s |= (c - '0');
        c = getchar();
    }
    unsigned int mask_1 = (1 << len); // used to set first bit to 1
    unsigned int mask_0 = (1 << len) - 1; // used to set first bit to 0
    while (c != '\n') {
        // insert cur string
        s &= mask_0; // set higher bits to 0
        s |= mask_1; // set first bit to 1
        // 1 is inserted at the front of s, so that (s-1) is just the bit representing the lowest node of s in the tree
        tree.insert_string(s);

        // rolling update
        s <<= 1;
        s |= (c - '0');
        start %= len;
        c = getchar();
    }
    // insert the rest of the string
    for (int i = 0; i < len; i++) {
        s &= mask_0;
        s |= mask_1;
        tree.insert_string(s);
        mask_0 >>= 1;
        mask_1 >>= 1;
    }

    // get result
    char res[50];
    len = tree.get_result(res);
    for (int i = len - 1; i >= 0; i--) {
        printf("%c", res[i]);
    }
    printf("\n");

    return 0;
}
