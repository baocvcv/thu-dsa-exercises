#include <cstdio>
#include <cstring>
#include <iostream>

// #define DEBUG

template<typename T> T read(T &x){
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

long *t;
int *l, *r;
int n;

long long *f;
long long *q;

int main(){
    read(n);
    l = new int[n+1];
    r = new int[n+1];
    t = new long[n+1];
    f = new long long[n+1];

    for(int i = 1; i < n; i++){
        read(l[i]);
        read(r[i]);
        read(t[i]);
    }

    q = new long long[n+1]; // index queue, increasing int [l, r]
    int ql = 1, qr = 0;
    f[1] = 0;
    int l_last = l[1], r_last = r[1];
    for(int k = 2; k <= n; k++){
        int l_cur = l_last;
        int r_cur = r_last;
        while(r[l_cur] < k){
            l_cur++;
        }
        while(l[r_cur] < k){
            r_cur++;
        }
        while(ql <= qr && q[ql] < l_cur){
            // pop item
            ql++;
        }
        for(int i = r_last; i <= r_cur; i++){
            while(ql <= qr && f[q[qr]] > f[r_cur]){
                qr--;
            }
            q[++qr] = i;
        }

        f[k] = f[q[1]] + t[q[1]];
    }

    printf("%lld\n", f[n]);
    return 0;
}