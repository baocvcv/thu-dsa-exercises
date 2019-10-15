// PA1-4 Jump
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX 1e16

int main(){
    // vars in the problem description
    int *t;
    int *l, *r;
    int n;

    // f for result
    long long *f;
    // q for a queue
    long long *q;

    scanf("%d", &n);
    l = new int[n+1];
    r = new int[n+1];
    t = new int[n+1];
    f = new long long[n+1];
    memset(f, 0, sizeof(long long) * (n+1));

    for(int i = 1; i < n; i++){
        scanf("%d %d %d", l+i, r+i, t+i);
    }
    t[n] = 0;

    q = new long long[n+1]; // 单调队列，用法参考了资料
    int qStart = 1, qEnd = 0;
    f[1] = t[1];

    int prevLeftBound= 1, prevRightBound= 1;
    for(int k = 2; k <= n; k++){
        // move to new section
        int curLeftBound = prevLeftBound;
        int curRightBound = prevRightBound;
        while((k < l[curLeftBound] || k > r[curLeftBound]) && curLeftBound < k){
            curLeftBound++; // move to the first point that contains k
        }
        if(curLeftBound >= k){
            // unreachable
            f[k] = MAX;
            continue;
        }

        if(curRightBound < curLeftBound){ // fast forward
            curRightBound = curLeftBound + 1;
        }
        while(l[curRightBound] <= k && k <= r[curRightBound] && curRightBound < k){
            curRightBound++; // move to the first point that cannot reach k
        }

        // update q
        while(qStart <= qEnd && q[qStart] < curLeftBound){
            // pop item left of curleftbound
            qStart++;
        }
        int i = curLeftBound > prevRightBound ? curLeftBound : prevRightBound;
        for(; i < curRightBound; i++){
            while(qStart <= qEnd && f[q[qEnd]] > f[i]){
                // pop items greater than f[i]
                qEnd--;
            }
            // insert f[i]
            q[++qEnd] = i;
        }

        // update f
        f[k] = f[q[qStart]] + t[k];

        // record boundaries
        prevLeftBound = curLeftBound;
        prevRightBound = curRightBound;
    }

    printf("%lld\n", f[n]);
    return 0;
}