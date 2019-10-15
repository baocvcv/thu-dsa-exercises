#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std;

template<typename T>inline T read(T &x){
    T data = 0;
    int sign = 1;
    char c = getchar();
    while(c != '-' && !isdigit(c)){
        c = getchar();
    }
    if(c == '-'){
        sign = -1;
        c = getchar();
    }
    while(isdigit(c)){
        data = data * 10 + c - '0';
        c = getchar();
    }
    x = data * sign;
    return x;
}

typedef long NumType;

NumType *x, *y;
int n;

int find(long px, long py){
    int l = 0, r = n - 1;
    if(px * y[r] + py * x[r] >= x[r] * y[r])
        return n;
    else if(px * y[l] + py * x[l] < x[l] * y[l])
        return 0;

    while(r - l > 1){
        int mid = (l + r)/2;
        NumType left = px * y[mid] + py * x[mid];
        NumType right = x[mid] * y[mid];
        if(left > right){
            l = mid;
        }else if(left < right){
            r = mid;
        }else{
            return mid + 1;
        }
    }
    return l + 1;
}

int main(){
    ios_base::sync_with_stdio(false);

    auto start = chrono::high_resolution_clock::now();

    read(n);
    x = new NumType[n];
    y = new NumType[n];
    for(int i = 0; i < n; i++){
        read(x[i]);
    }
    auto cmp = [](const void *a, const void *b){
        return (*(long*)a > *(long*)b) ? 1 : -1;
    };
    qsort(x, n, sizeof(x[0]), cmp);
    for(int i = 0; i < n; i++){
        read(y[i]);
    }
    qsort(y, n, sizeof(y[0]), cmp);

    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(stop - start);
    printf("Sort Time: %d\n", time);

    int m;
    read(m);
    for(int i = 0; i < m; i++){
        NumType px, py;
        read(px);
        read(py);
        // printf("%d\n", find(px, py));
    }

    stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::milliseconds>(stop - start);
    printf("Time: %d\n", time);

    return 0;
}