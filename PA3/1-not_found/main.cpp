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

// use bitmap to represent a complete binary tree
// edge represents chars, going left = 0, going right = 1
// node denotes if the path from parent to child has been taken
struct BitmapTree {
    int total_len;
    char* data;

    BitmapTree (int _len) {
        total_len = (_len + 7) >> 3;
        data = new char[total_len];
        reset();
        set_bit(0); // set root node to 1
    }
    inline void reset() { memset(data, 0, sizeof(char) * total_len); }
    inline void set_bit(int k) { data[k >> 3] |= bytes[k & 0x07]; }
    inline int check(int k) { return (data[k >> 3] & bytes[k & 0x07]) != 0; }

    inline void insert_string(char* s, int len_s, int start, int len_target) { // insert string into tree
        // for (int i = 0; i < len_target; i++) { printf("%c", s[(start + i) % len_s]); } printf("\n");

        int cur_bit = 0; // used to track parent bit position, must have been set to 1
        for (int i = 0; i < len_target; i++) {
            int dir = (s[(start + i) % len_s] == '1'); // go right if dir == 1, left if dir == 0
            int next_bit = (cur_bit << 1) + 1 + dir; // pos of the next bit
            set_bit(next_bit);
            cur_bit = next_bit;
        }
    }

    int get_result(char* res) { // res used to store the result, in recerse order
        // find the first 0
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
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
#endif

    BitmapTree tree(1 << 25);

    // read first 24 chars
    int len, start;
    char s[25];
    char c = getchar();
    for(len = start = 0; len < 24 && c != '\n'; len++) {
        s[len] = c;
        c = getchar();
    }
    s[len] = 0;
    while (c != '\n') {
        // insert cur string
        tree.insert_string(s, len, start, len);
        // rolling update
        s[start++] = c;
        start %= len;
        c = getchar();
    }
    // insert the rest of the string
    for (int i = 0; i < len; i++) {
        tree.insert_string(s, len, (start + i) % len, len - i);
        // tree.print_line(1 << 8);
    }
    // tree.print_line(1 << 8);

    // get result
    len = tree.get_result(s);
    for (int i = len - 1; i >= 0; i--) {
        printf("%c", s[i]);
    }
    printf("\n");

    return 0;
}
