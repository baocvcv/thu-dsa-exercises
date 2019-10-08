#include <cstdio>
#include <cstring>
#include <cmath>

#define isdigit(x) ('0' <= (x) && (x) <= '9')

template <typename T> 
inline T read(T &x){
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
    while(isdigit(c)){
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= sig;
    return x;
}

struct Node {
    char _c;
    Node* _next;

    Node(char c, Node* next){
        _c = c;
        _next = next;
    }
};

struct List {
    Node* _head;
    Node* _tail;
    int _size;

    List() {
        _head = new Node(0, 0);
        _tail = _head;
    }
};

List array[800];
int M = 1;
int N;

bool merge(){
    if(N < 3)
        return false;

    bool flag = false;
    Node* h_prev = array[0]._head;
    while(h_prev->_next != 0){
        Node* h_cur = h_prev->_next;
        int counter = 1;
        // look for mergable section
        bool local_flag = false;
        while(h_cur->_next != 0 && h_cur->_next->_c == h_prev->_next->_c){
            counter++;
            h_cur = h_cur->_next;
            if(counter > 2){
                flag = true;
                local_flag = true;
                break;
            }
        }

        if(local_flag){
            // merge
            //TODO: delete merged nodes
            h_prev->_next = h_cur->_next;
        }else{
            h_prev = h_cur;
        }
    }
    return flag;
}

void rebuild(bool force = false){
    if(N <= 0)
        return;

    int blockSize = sqrt(N);
    if(!force){
        bool flag = false; // is rebuild necessary
        for(int i = 0; i < M; i++){
            if(array[i]._size > blockSize*10){
                flag = true;
                break;
            }
        }
        if(!flag)
            return;
    }

    // split
    Node* h = array[0]._head->_next;
    for(int i = 1, M = 1; i < N; i++){
        if(i % blockSize == 0){
            array[M-1]._tail = h; 
            array[M-1]._size = blockSize;
            array[M]._head->_next = h->_next;
            M++;
        }
        h = h->_next;
    }
    array[M-1]._tail = h;
    array[M-1]._size = N % blockSize;
}

void add(int pos, char c){
    if(pos > N){
        printf("Error: pos %d > N %d\n", pos, N);
        return;
    }

    int x = 0; // the block pos is in
    while(array[x]._size < pos){
        pos -= array[x]._size;
        x++;
    }

    Node* h = array[x]._head;
    int n = 0;
    while(n < pos){
        // move to pos
        h = h->_next;
        n++;
    }

    // insert
    Node* node = new Node(c, h->_next);
    h->_next = node;
    if(array[x]._tail == h) // added to tail
        array[x]._tail = node;
    array[x]._size++;
    N++;

    // merge & rebuild
    while(merge())
        ;
    rebuild();
}

void print(){
    printf("\n******************\n");
    for(int i = 0; i < M; i++){
        Node* h = array[i]._head->_next;
        while(h != array[i]._tail->_next){
            printf("%c", h->_c);
            h = h->_next;
        }
        printf("  %d\n", array[i]._size);
    }
    printf("\n");
}

int main(){
    // read initial string 
    char c = getchar();
    while(c != '\n'){
        array[0]._tail->_next = new Node(c, 0);
        array[0]._tail = array[0]._tail->_next;
        N++;
        c = getchar();
    }
    array[0]._size = N;
    rebuild();

    print();

    // ops
    int n;
    read(n);
    for(int i = 0; i < n; i++){
        int pos;
        char c;
        read(pos);
        c = getchar();
        add(pos, c);
        print();
    }

    Node* h = array[0]._head->_next;
    while(h != 0){
        printf("%c", h->_c);
        h = h->_next;
    }
    printf("\n");

    return 0;
}