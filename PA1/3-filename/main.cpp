#include <cstdio>
#include <cstring>

int main(){
    int N, M, K;
    char source[501005], target[501005];
    int dp[501005];

    scanf("%d%d%d", &N, &M, &K);

    // start from pos 1
    scanf("%s", source+1);
    scanf("%s", target+1);

    for(int i = 0; i <= N; i++){ // i = 0
        dp[i] = i;
    }

    int i, j;
    bool flag = false;
    for(j = 1; j <= M; j++){
        int i0 = (j < 106) ? 1 : j - 105;
        int p0 = dp[i0 - 1]; // p[i-1, j-1]
        for(i = i0; i <= j + 105 && i <= N; i++){
            int tmp = dp[i]; // tmp = p[i, j-1]
            if(source[i] == target[j]){
                dp[i] = p0;
            }else{
                int p1 = (i0 == 1 || i > i0) ? dp[i-1] + 1 : K+1; // delete, p[i-1, j]
                int p2 = (i < j+105) ? dp[i] + 1 : K+1; // insert, p[i, j-1]
                dp[i] = (p1 < p2) ? p1 : p2;
            }
            p0 = tmp;
        }
    }
    if(j > M && i > N)
        flag = true;

    if(flag && dp[N] <= K)
        printf("%d\n", dp[N]);
    else
        printf("-1\n");

    return 0;
}