#include <stdio.h>
#include <string.h>
#include "crc32.h"

#define ull unsigned long long

int main() {
    ull num;
    scanf("%llx", &num);

    unsigned char *s = (unsigned char *)&num;
    for (int i = 0; i < 8; i++)
        printf("%c ", s[i]);
    printf("\n");

    unsigned char m[15];
    scanf("%s", m);
    printf("%x\n", crc32(0, m, strlen((const char*)m)));

    return 0;
}