#include <cstdio>
#include <cstring>

#define MAX 1e16
// #define DEBUG

using namespace std;

template <typename T>
inline T read(T &x){
    T data = 0;
    int sgn = 1;
    char c = getchar();
    while(c != '-' && !isdigit(c))
        c = getchar();
    if(c == '-'){
        sgn = -1;
        c = getchar();
    }
    while(isdigit(c)){
        data = data * 10 + c - '0';
        c = getchar();
    }
    x = data * sgn;
    return x;
}

int main(){
    // vars in the problem description
    long long *t;
    int *l, *r;
    int n;

    // f for result
    long long *f;
    // q for a queue
    long long *q;

    read(n);
    l = new int[n+1];
    r = new int[n+1];
    t = new long long[n+1];
    f = new long long[n+1];

    for(int i = 1; i < n; i++){
        read(l[i]);
        read(r[i]);
        read(t[i]);
    }

    q = new long long[n+1]; // index queue, increasing int [l, r]
    int qStart = 1, qEnd = 0;
    f[1] = 0;

    int prevLeftBound= 1, prevRightBound= 1;
    for(int k = 2; k <= n; k++){
        // move to new section
        int curLeftBound = prevLeftBound;
        int curRightBound = prevRightBound;
        while((k < l[curLeftBound] || k > r[curLeftBound]) && curLeftBound < k){
            curLeftBound++;
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

        while(qStart <= qEnd && q[qStart] < curLeftBound){
            // pop item left of curleftbound
            qStart++;
        }
        int i = curLeftBound > prevRightBound ? curLeftBound : prevRightBound;
        for(; i <= curRightBound; i++){
            while(qStart <= qEnd && f[q[qEnd]] > f[curRightBound]){
                qEnd--;
            }
            q[++qEnd] = i;
        }

        // update f
        f[k] = f[q[1]] + t[q[1]];

        // update boundaries
        prevLeftBound = curLeftBound;
        prevRightBound = curRightBound;
    }

    printf("%lld\n", f[n]);
    return 0;
}