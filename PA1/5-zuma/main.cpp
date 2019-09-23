#include <cstdio>
#include <cstring>

template <typename T> 
T read(T &x){
    char c;
    int sig = 1;
    x = 0;

    c = getchar();
    while(c != '-' && !isdigit(c))
        c = getchar();
    if(c == '-'){
        sig = -1;
        c = getchar();
    }
    
}