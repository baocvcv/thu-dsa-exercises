#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int stoi(const char* s, int len = 4)
{
    int res = 0;
    for(int i = 0; i < len; i++){
        res = res * 10 + s[i] - '0'; 
    }
    return res;
}

// unsigned long 20 digits, each part uses 9 digits
int multiply(const char* op1, const char* op2, unsigned int* res)
{
    int n1 = strlen(op1);
    int n2 = strlen(op2);
    int n = n1/4 + n2/4 + 3;
    for (int i = 0; i < n; i++)
        res[i] = 0;

    int i, j;
    for (i = n2-4; i >= 0; i-=4){
        int x1 = stoi(op1+i);
        for(j = n1-4; j >= 0; j-=4){
            int x2 = stoi(op2+j);
            int pos = i/4 + j/4 + 5;
            res[pos] += x1 * x2;
            res[pos-1] += res[pos] / 10000;
            res[pos] %= 10000;
        }
        if(j < 0){
            j += 4;
            int x2 = stoi(op2, j);
            int pos = i/4 + j/4 + 5;
            res[pos] += x1 * x2;
            res[pos-1] += res[pos] / 10000;
            res[pos] %= 10000;
        }
    }
    if(i < 0){
        int x1 = stoi(op1, i+4);
        for(j = n1-4; j >= 0; j-=4){
            int x2 = stoi(op2+j);
            int pos = i/4 + j/4 + 5;
            res[pos] += x1 * x2;
            res[pos-1] += res[pos] / 10000;
            res[pos] %= 10000;
        }
        if(j < 0){
            j += 4;
            int x2 = stoi(op2, j);
            int pos = i/4 + j/4 + 5;
            res[pos] += x1 * x2;
            res[pos-1] += res[pos] / 10000;
            res[pos] %= 10000;
        }
    }

    return n;
}

int main() {
    int n;
    cin >> n;

    char op1[5005], op2[5005];
    for(int i = 0; i < n; i++){
        cin >> op1 >> op2;
        unsigned int res[3000];
        int len = multiply(op1, op2, res);
        int j = 0;
        while(res[j] == 0)
            j++;
        while(j < len)
            cout << res[j++];
        cout << endl;
    }
    return 0;
}