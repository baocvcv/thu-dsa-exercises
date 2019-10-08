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
    merge();
    rebuild();
}

bool merge(){
    if(N < 3)
        return false;

    // TODO: possible solution
    /* if >3 consecutive chars detected,
    connect the lists into a single list and merge
    after merge is complete, rebuild
    */

    // another thought:
    // only check the pos where new bead is added
    // this requires two-sided linked list

    bool flag = false;
    Node* h_prev = array[0]._head->_next;
    int curBlock = 0;
    int nextBlock = 1;
    // check if merge is needed
    do{ // while(h != 0)
        Node* h_cur = h_prev->_next;
        if(h_cur == 0){ // reaches the end of the list
            while(nextBlock < M && array[nextBlock]._size <= 0){
                nextBlock++;
            }
            if(nextBlock < M){
                h_cur = array[nextBlock]._head->_next;
                nextBlock++;
            }else{
                // h_cur == 0 && array[M-1]._head == 0
                break;
            }
        }

        int counter = 1;
        while(h_cur->_c == h_prev->_c){
            counter++;
            if(counter > 2){
                flag = true;
                break;
            }

            h_cur = h_cur->_next;
            if(h_cur == 0){
                while(nextBlock < M && array[nextBlock]._size <= 0){
                    nextBlock++;
                }
                if(nextBlock < M){
                    h_cur = array[nextBlock]._head->_next;
                    nextBlock++;
                }else{
                    // h_cur == 0 && array[M-1]._head == 0
                    break;
                }
            }
        }
        if(counter >= 3)
            break;
        h_prev = h_cur;
    }while(h != 0 && nextBlock < M);

    if(flag){ // require merging
        // gather all the nodes
        Node* tail = array[0]._tail;
        for(int i = 1; i < M && tail != 0; i++){
            tail->_next = array[i]._head->_next;
            tail = array[i]._tail;
            array[i]._head->_next = 0;
            array[i]._tail = array[i]._head;
            array[i]._size = 0;
        }

        // TODO: delete nodes

    

    }

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
        tail->_next = array[i]._head->_next;
        tail = array[i]._tail;
        array[i]._head->_next = 0;
        array[i]._tail = array[i]._head;
        array[i]._size = 0;
    }

    // split
    M = N / blockSize + 1;
    Node* h = array[0]._head->_next;
    for(int i = 1, n = 1; i < N; i++){
        if(i % blockSize == 0){
            array[n-1]._tail = h; 
            array[n-1]._size = blockSize;
            array[n]._head->_next = h->_next;
            n++;
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
    char c = getchar();
    List &arr = array[0];
    while(c != '\n'){
        arr._tail->_next = new Node(c, 0);
        arr._tail = arr._tail->_next;
        arr._size++;
    }
    N = arr._size;

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
            Node* h = array[i]._head->_next;
            while(h != 0){
                printf("%c", h->_c);
                h = h->_next;
            }
        }
    }
    printf("\n");
}