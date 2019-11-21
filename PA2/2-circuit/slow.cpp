#include <cstdio>
#include <cstdlib>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ll unsigned long long

int main () {
    int n, k;
    scanf("%d %d", &n, &k);

    ll *data = new ll[n+2];
    char s[70];
    for (int i = 0; i < n; i++) {
        scanf("%s", s);
        data[i] = 0;
        for (int j = 0; j < 64; j++) {
            data[i] = data[i] * 2 + s[j] - '0';
        }
    }

    for (int i = 0 ; i < n; i++) {
        // printf("data[i]: %llu:\n", data[i]);
        ll res = 0;
        int pos;
        int start = MAX(0, i - k - 1);
        int finish = MIN(n-1, i + k + 1);
        for (int j = start; j <= finish; j++) {
            ll tmp = data[i] ^ data[j];
            // printf("   %llu = %llu\n", data[j], tmp);
            if (tmp > res) {
                res = tmp;
                pos = j;
            }
        }
        printf("%d\n", pos);
    }

    return 0;
}