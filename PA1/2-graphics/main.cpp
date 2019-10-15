// PA1-2 Graphics
#include <cstdlib>
#include <cstdio>

using namespace std;

int main(){
    long long *x, *y;
    int n;
    scanf("%d", &n);
    x = new long long[n];
    y = new long long[n];

    // read input
    for(int i = 0; i < n; i++){
        scanf("%lld", x+i);
    }
    auto cmp = [](const void *a, const void *b){
        return (*(long long*)a - *(long long*)b) > 0 ? 1 : -1;
    };
    // sort the x-coordinates
    qsort(x, n, sizeof(x[0]), cmp);
    for(int i = 0; i < n; i++){
        scanf("%lld", y+i);
    }
    // sort the y-coordinates
    qsort(y, n, sizeof(y[0]), cmp);

    // binary search
    auto find = [&](long long px, long long py){
        int l = 0, r = n - 1;
        // corner cases
        if(px * y[r] + py * x[r] >= x[r] * y[r])
            return n;
        else if(px * y[l] + py * x[l] < x[l] * y[l])
            return 0;

        // search
        while(r - l > 1){
            int mid = (l + r)/2;
            long long left = px * y[mid] + py * x[mid];
            long long right = x[mid] * y[mid];
            if(left > right){
                l = mid;
            }else if(left < right){
                r = mid;
            }else{
                return mid + 1;
            }
        }
        return l + 1;
    };

    int m;
    scanf("%d", &m);
    for(int i = 0; i < m; i++){
        long long px, py;
        scanf("%lld%lld", &px, &py);
        int res = find(px, py);
        printf("%d\n", res);
    }

    return 0;
}