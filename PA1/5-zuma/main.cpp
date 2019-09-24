#include <cstdio>
#include <cstring>

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
    int _size;

    List() {
        _head = 0;
        _size = 0;
    }
};

List array[800];
int N;

void add(int pos, char c){
    int x = 0; // the block pos is in
    while(array[x]._size <= pos){
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
    array[x]._size++;
    N++;

    // merge & rebuild
    while(merge())
        ;
    rebuild();
}

bool merge(){

}

void rebuild(){

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
    array[0]._size = size;
    N = size;

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
    }

    for(int i = 0; i < 800; i++){
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