#include <cstdio>
#include <cstring>

int main() {
    unsigned int i = 0;
    char c = getchar();
    while(c == '0' || c == '1') {
        i++;
        c = getchar();
    }

    if (i > 255)
        return -1;
    else
        return i;
}