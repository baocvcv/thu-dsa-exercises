#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "crc32.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll long long
#define ull unsigned long long
#define u unsigned int
#define BUCKET_LEN 2946901
#define SECRET_LEN 5

union string {
    ull l;
    unsigned char s[8];

    string() { l = 0; }
    string(int a) { l = a; }

    inline int len() {
        int l = 0;
        while (l < 8 && s[l]) l++;
        return l;
    }

    // print the string represented by s
    void print() {
        for (int i = 0; i < 8 && s[i]; i++) putchar(s[i]);
        putchar('\n');
    }
};

struct BinNode {
    string s; // used to store the password
    u crc;
    // whether the crc code is duplicate
    bool is_duplicate;
    BinNode *lc, *rc;

    BinNode(): lc(0), rc(0), is_duplicate(false) {}
};

// a binary tree to save crc32 values
struct BinTree {
    BinNode *root;

    BinTree(): root(0) {}

    // return 0 if not found
    BinNode** get(u crc) {
        return get(crc, &root);
    }

    // search the binary tree for crc
    BinNode** get(u crc, BinNode **cur) {
        if (*cur == 0) return cur;

        ull cur_crc = (*cur)->crc;
        if (cur_crc == crc) return cur;
        if (cur_crc < crc) return get(crc, &((*cur)->lc));
        else return get(crc, &((*cur)->rc));
    }

    // insert into tree
    void insert(u crc, string s) {
        BinNode** node = get(crc);
        if (*node == 0) {
            *node = new BinNode();
            (*node)->crc = crc;
            (*node)->s = s;
        } else if ((*node)->s.l != s.l) {
            (*node)->is_duplicate = true;
        }
    }

    void print_tree() {
        print_at(root);
    }

    void print_at(BinNode *node) {
        if (node == 0) return;
        printf("%x %d %llx ", node->crc, node->is_duplicate, node->s);
        node->s.print();
        print_at(node->lc);
        print_at(node->rc);
    }
};

BinTree bucket[BUCKET_LEN];
unsigned char salt[5];
unsigned int salt_len;

unsigned char letters[] = "0123456789tsinghua";
const int len = strlen((char *)letters); //  strlen_str(letters);

// calculates crc with salt
u get_crc(string secret, u len) {
    u crc = crc32(0, secret.s, len);
    return crc32(crc, salt, salt_len);
}

// compute all the passwords with length <= 5
void compute(string secret, unsigned char* new_letter, int level, u base) {
    u crc_base = 0;
    if (level > 0) { // len > 0
        crc_base = crc32(base, new_letter, 1);
        u crc = crc32(crc_base, salt, salt_len);
        bucket[crc % BUCKET_LEN].insert(crc, secret);
    }

    if (level < SECRET_LEN) { // len < 5
        for (int i = 0; i < len; i++) {
            secret.s[level] = letters[i];
            compute(secret, letters + i, level + 1, crc_base);
        }
    }
}

void debug_info() {
    for (u i = 0; i < BUCKET_LEN; i++) {
        if (bucket[i].root != 0) {
            printf("Bucket %d\n", i);
            bucket[i].print_tree();
            printf("\n");
        }
    }
}

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
#ifdef _OJ_
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
#endif

    int n;
    scanf("%d", &n);
    scanf("%s", salt);
    salt_len = strlen((char*)salt);

    // add all 5-digit passwords
    compute(0, 0, 0, 0);

    // go through all the secrets
    u crc;
    // records weak passwords of length 6,7,8 respectively
    string history[3] = {0, 0, 0};
    // whether weak passwords of length 6,7,8 have been recorded
    bool ready[3] = {false, false, false};
    int i;
    for (i = 0; i < n; i++) {
        scanf("%x", &crc);
        BinNode** res = bucket[crc % BUCKET_LEN].get(crc);
        if (*res == 0) {
            printf("No\n");
        } else {
            if ((*res)->is_duplicate) {
                printf("Duplicate\n");
            } else {
                (*res)->s.print();
                // record first letter
                char letter = (*res)->s.s[0];
                for (int j = 0; j < 3; j++) {
                    int target_len = j + 6; // length of 6, 7, 8 respectively
                    int actual_len = history[j].len();
                    if (actual_len < target_len - 1) { // simply append
                        history[j].s[actual_len] = letter;
                    } else if (actual_len == target_len - 1) {
                        history[j].s[actual_len] = letter;
                        ready[j] = true;
                        u crc = get_crc(history[j], target_len);
                        bucket[crc % BUCKET_LEN].insert(crc, history[j]);
                    } else if (actual_len >= target_len) {
                        history[j].l >>= 8;
                        history[j].s[target_len - 1] = letter; // shift and append
                        u crc = get_crc(history[j], target_len);
                        bucket[crc % BUCKET_LEN].insert(crc, history[j]);
                    }
                }
                if (ready[2]) { // all the lengths have been accounted for
                    i++;
                    break;
                }
            }
        }
    }
    for (; i < n; i++) {
        scanf("%x", &crc);
        BinNode** res = bucket[crc % BUCKET_LEN].get(crc);
        if (*res == 0) {
            printf("No\n");
        } else {
            if ((*res)->is_duplicate) {
                printf("Duplicate\n");
            } else {
                (*res)->s.print();
                // record the first letter
                char letter = (*res)->s.s[0];
                for (int j = 0; j < 3; j++) {
                    int target_len = j + 6; // length of 6, 7, 8 respectively
                    history[j].l >>= 8;
                    history[j].s[target_len - 1] = letter; // shift and append
                    u crc = get_crc(history[j], target_len);
                    bucket[crc % BUCKET_LEN].insert(crc, history[j]);
                }
            }
        }
    }

    return 0;
}
