#include <cstdio>

int main() {
    int len = 1;
    char c = getchar();
    while ('a' <= c && c <= 'z') {
        len++;
        c = getchar();
    }
    return len / 1e6;
}