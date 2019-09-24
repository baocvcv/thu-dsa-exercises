#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

const int LEN = 8;
const int BUF_SIZE = 1500;
const long long MOD = pow(10, LEN);

inline long long stoi(const char* s, int len = LEN)
{
    long long res = 0;
    for(int i = 0; i < len; i++){
        res = res * 10 + s[i] - '0'; 
    }
    return res;
}

// unsigned long 20 digits, each part uses 9 digits
int multiply(const char* op1, const char* op2, long long* res)
{
    int n1 = strlen(op1);
    int n2 = strlen(op2);
    long long a[BUF_SIZE], b[BUF_SIZE];
    memset(a, 0, sizeof(long long) * BUF_SIZE);
    memset(b, 0, sizeof(long long) * BUF_SIZE);

    // convert to LEN-digit numbers
    int lenA = 0;
    for(int i = n1; i > 0; i -= LEN){
        int j = (i - LEN) > 0 ? i - LEN : 0;
        a[lenA++] = stoi(op1 + j, i - j);
    }
    int lenB = 0;
    for(int i = n2; i > 0; i -= LEN){
        int j = (i - LEN) > 0 ? i - LEN : 0;
        b[lenB++] = stoi(op2 + j, i - j);
    }

    // perform mulitply
    for(int i = 0; i < lenA; i++){
        for(int j = 0; j < lenB; j++){
            res[i + j] += a[i] * b[j];
        }
    }

    // move up 
    for(int i = 0; i < lenA + lenB; i++){
        res[i + 1] += res[i] / MOD;
        res[i] %= MOD;
    }

    return lenA + lenB;
}

int main() {
    int n;
    scanf("%d", &n);

    char op1[5005], op2[5005];
    for(int i = 0; i < n; i++){
        scanf("%s %s", op1, op2);
        long long res[5000];
        memset(res, 0, sizeof(long long) * 5000);
        int len = multiply(op1, op2, res);

        // walk through 0s
        while(res[len] == 0 && len > 0)
            len--;
        // first "digit"
        printf("%lld", res[len]);
        len--;
        // rest of the "digits"
        while(len >= 0){
            printf("%08lld", res[len]);
            len--;
        }
        printf("\n");
    }

    return 0;
}