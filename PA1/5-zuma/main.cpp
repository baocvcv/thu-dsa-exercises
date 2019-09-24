#include <cstdio>
#include <cstring>
#include <cmath>

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
        _head = 0;
        _size = 0;
    }
};

List array[800];
int M = 1;
int N;

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

bool merge(){
    if(N == 0)
        return false;

    bool flag = false;
    Node* h_prev = array[0]._head;
    Node* h = h_prev;
    int curBlock = 0;
    int nextBlock = 1;
    int counter = 1;
    while(h != 0){
        Node* p = h->_next;
        if(p == 0){
            if(array[nextBlock]._head != 0){
                p = array[nextBlock]._head;
                nextBlock++;
            }else{
                // p == 0 && array[nextBlock]._head == 0
                break;
            }
        }
        while(p->_c == h->_c){
            counter++;
            p = p->_next;
            if(p == 0){
                if(array[nextBlock]._head != 0){
                    p = array[nextBlock]._head;
                    nextBlock++;
                }else{
                    // p == 0 && array[nextBlock]._head == 0
                    break;
                }
            }
        }
        if(counter >= 3){
            Node *tmp = h->_next;
            while(h != p){
                delete h;
                h = tmp;
                tmp = h->_next;
            }
            h_prev->_next = h;
            flag = true;
        }else{
            while(h_prev->_next != p){
                h_prev = h_prev->_next;
            }
            h = p;
        }
    }
    //TODO: update block info!!!!!!!!!??????????????

    return flag;
}

void rebuild(){
    int blockSize = sqrt(N);
    bool flag = false; // is rebuild necessary
    for(int i = 0; i < M; i++){
        if(array[i]._size > blockSize*10){
            flag = true;
            break;
        }
    }
    if(!flag)
        return;

    // merge
    Node* tail = array[0]._tail;
    for(int i = 1; i < M && tail != 0; i++){
        tail->_next = array[i]._head;
        tail = array[i]._tail;
        array[i]._head = 0;
        array[i]._size = 0;
        array[i]._tail = 0;
    }

    // split
    M = N / blockSize + 1;
    Node* h = array[0]._head;
    for(int i = 1; i < N; i++){
        if(i % blockSize == 0){
            int n = i / blockSize;
            array[n-1]._tail = h; 
            array[n-1]._size = blockSize;
            array[n]._head = h->_next;
        }
        h = h->_next;
    }
    while(h->_next != 0)
        h = h->_next;
    array[N / blockSize]._tail = h;
    array[N / blockSize]._size = N % blockSize;
}

void print(){
    printf("******************\n");
    for(int i = 0; i < M; i++){
        Node* h = array[i]._head;
        while(h != 0){
            printf("%c", h->_c);
            h = h->_next;
        }
        printf("  %d\n", array[i]._size);
    }
    printf("******************\n");
}

int main(){
    // read initial string 
    Node* head = 0;
    Node* tail = 0;
    int size = 0;
    char c = getchar();
    while(c != '\n'){
        if(head == 0){
            head = new Node(c, 0);
            tail = head;
        }else{
            tail->_next = new Node(c, 0);
            tail = tail->_next;
        }
        size++;
    }
    array[0]._head = head;
    array[0]._tail = tail;
    array[0]._size = size;
    N = size;

    print();

    // ops
    int n;
    read(n);
    for(int i = 0; i < n; i++){
        int pos;
        char c;
        read(pos);
        getchar(); // skip ' '
        c = getchar();

        add(pos, c);

        print();
    }

    for(int i = 0; i < M; i++){
        if(array[i]._size > 0){
            Node* h = array[i]._head;
            while(h != 0){
                printf("%c", h->_c);
                h = h->_next;
            }
        }
    }
    printf("\n");
}