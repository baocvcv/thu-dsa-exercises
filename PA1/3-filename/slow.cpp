#include <cstdio>
#include

int N, M, K;
char source[501005], target[501005];
int dp[501005];




int main(){
    scanf("%d%d%d", &N, &M, &K);
    // start from pos 1
    scanf("%s", source+1);
    scanf("%s", target+1);

    // abs
    auto abs = [](int a){
        return a > 0 ? a : -a;
    };

    auto min = [](int a, int b, int c){
        if(a < b)
            return (a < c) ? a : c;
        else
            return (b < c) ? b : c;
    };
}