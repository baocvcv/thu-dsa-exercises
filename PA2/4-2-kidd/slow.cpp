#include <cstdio>
#include <cstdlib>

using namespace std;


int main() {
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

    unsigned int n,m;
    scanf("%d %d", &n, &m);
    long long *ans = new long long[n+1];

    for (int i = 0; i < m; i++) {
        char s[3];
        long long x, y;
        scanf("%s", s);
        scanf("%lld %lld", &x, &y);
        if (s[0] == 'Q') {
            long long sum = 0;
            for (int j = x; j <= y; j++) sum += ans[j];
            printf("%lld\n", sum);
        } else {
            for (int j = x; j <= y; j++) ans[j]++;
        }
    }
    return 0;
}
