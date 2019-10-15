// PA1-3 Filename
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

    // p[i,j]: the min no. of ops required to
    // transform source[1..i+1] to target[1..j+1]
    for(int j = 0; j <= N; j++){ // i = 0
        dp[j] = j;
    }
    int i, j;
    bool flag = false;
    for(j = 1; j <= M; j++){
        int i0 = (j < K + 1) ? 1 : j - K; // do not consider i < j - K
        int p0 = dp[i0 - 1]; // p[i-1,j-1]
        // 与陈宇翔就该优化进行过讨论
        for(i = i0; i <= j + K && i <= N; i++){ // not consider i > j+105
            int tmp = dp[i]; // tmp = p[i,j-1]
            if(source[i] == target[j]){ // s[i] == t[j]
                dp[i] = p0;
            }else{
                int p1 = (i0 == 1 || i > i0) ? dp[i-1] + 1 : K+1; // delete, p[i-1, j]
                int p2 = (i < j + K) ? dp[i] + 1 : K+1; // insert, p[i, j-1]
                dp[i] = (p1 < p2) ? p1 : p2; // min{ p[i-1,j], p[i,j-1] }
            }
            p0 = tmp;
        }
    }
    if(j > M && i > N) // dp covered the p[M,N] square
        flag = true;

    if(flag && dp[N] <= K)
        printf("%d\n", dp[N]);
    else
        printf("-1\n");

    return 0;
}