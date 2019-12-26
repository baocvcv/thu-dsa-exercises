#include <cstdio>
#include <cstdlib>
#include <cctype>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll long long
#define MAXN 20000005

char input[MAXN]; // records the input string
int next[MAXN]; // the 'next' array
int val[MAXN]; // val[i] = how many prefixes ending at position i are there

int main() {
#ifndef _OJ_
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
#ifdef _OJ_
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
#endif

    // init values
    int len = 0;
    val[0] = 1;
    next[0] = -1;
    long long total_num = 0; // stores the result
    int t = -1; // = -1

    // start reading input and calculate
    char c = getchar();
    while(c != EOF && islower(c)) {
        if (t < 0) {
            input[len] = c;
            val[len] = 1;
            total_num += val[len];
            next[++len] = ++t;
            c = getchar();
        } else if (input[t] == c) {
            input[len] = c;
            val[len] = val[t] + 1; // accumulate vals
            total_num += val[len];
            next[++len] = ++t;
            c = getchar();
        } else {
            t = next[t];
        }
    }
    printf("%lld\n", total_num);
    return 0;
}
