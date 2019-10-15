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
int max_size = 0;

void print(){
    for(int i = 0; i < M; i++){
        Node* h = array[i]._head->_next;
        for(int j = 0; j < array[i]._size; j++){
            printf("%c", h->_c);
            h = h->_next;
        }
        printf("  %d\n", array[i]._size);
    }
    printf("\n");
}

bool merge(){
    if(N < 3)
        return false;

    bool flag = false;
    Node* h_prev = array[0]._head;
    while(h_prev->_next != 0){
        Node* h_cur = h_prev->_next;
        int counter = 1;
        // look for mergable section
        while(h_cur->_next != 0 && h_cur->_next->_c == h_prev->_next->_c){
            counter++;
            // printf("%c %c %d\n", h_prev->_next->_c, h_cur->_next->_c, counter);
            h_cur = h_cur->_next;
        }

        if(counter > 2){
            flag = true;
            N -= counter;
            //delete nodes
            Node *h_tmp1 = h_prev->_next;
            Node *h_tmp2 = h_tmp1->_next;
            for (; counter > 0; counter--){
                // printf("%c", h_tmp1->_c);
                delete h_tmp1;
                h_tmp1 = h_tmp2;
                if(h_tmp2 != 0)
                    h_tmp2 = h_tmp2->_next;
             }
             // printf("\n");
            h_prev->_next = h_tmp1;
        }else{
            h_prev = h_cur;
        }
    }
    return flag;
}

void rebuild(bool force = false){
    int blockSize = sqrt(N);
    if(!force){
       if(max_size < blockSize * 4)
            return;
    }

    // split
    Node* h = array[0]._head->_next;
    M = 1;
    for(int i = 1; i < N; i++){
        if(i % blockSize == 0){
            array[M-1]._tail = h; 
            array[M-1]._size = blockSize;
            array[M]._head->_next = h->_next;
            M++;
        }
        h = h->_next;
    }
    array[M-1]._tail = h;
    if(blockSize == 0){
        array[M-1]._size = 0;
    }else{
        array[M-1]._size = N % blockSize == 0 ? blockSize : N % blockSize;
    }

    // change max_size due to rebuild
    max_size = blockSize;
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
    if(array[x]._size > max_size){
        max_size = array[x]._size;
    }

    // print();

    // merge & rebuild
    bool flag = false;
    while(merge())
        flag = true;
    rebuild(flag);
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
    max_size = N;
    rebuild();

    // ops
    int n;
    read(n);
    for(int i = 0; i < n; i++){
        int pos;
        char c;
        read(pos);
        c = getchar();
        // printf("%d %c\n", pos, c);
        add(pos, c);
        // print();
    }

    Node* h = array[0]._head->_next;
    while(h != 0){
        printf("%c", h->_c);
        h = h->_next;
    }
    printf("\n");

    return 0;
}