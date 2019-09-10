#include <iostream>
#include <string>
#include <cstring>

using namespace std;

void multiply(const char* op1, const char* op2, char* res)
{
    int n1 = strlen(op1);
    int n2 = strlen(op2);
    int n = n1 + n2 + 1;
    int *tmp = new int[n];
    for (int i = 0; i < n; i++)
        tmp[i] = 0;
    for (int i = n2-1; i >= 0; i--){
        for(int j = n1-1; j >= 0; j--){
            tmp[i+j+2] += (op2[i] - '0') * (op1[j] - '0');
            tmp[i+j+1] += tmp[i+j+2] / 10;
            tmp[i+j+2] %= 10;
        }
    }
    int i = 0;
    while(tmp[i] == 0)
        i++;
    int j = 0;
    for(; i < n; i++, j++){
        res[j] = tmp[i] + '0';
    }
    res[j] = '\0';
}

int main() {
    int n;
    cin >> n;

    char op1[5005], op2[5005];
    for(int i = 0; i < n; i++){
        cin >> op1 >> op2;
        char res[10005];
        multiply(op1, op2, res);
        cout << res << endl;
    }
    return 0;
}