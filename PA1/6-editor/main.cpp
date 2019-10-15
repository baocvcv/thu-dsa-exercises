// PA1-6 Perfect Editor
#include <cstdio>

/*
A node in the linked list
*/
struct Node {
    char c;
    Node* p1;
    Node* p2;

    // Return the pointer different from p
    inline Node* other(Node* p) {
        if (p == p1)
            return p2;
        else
            return p1;
    }

    // Modify the pointer from p to p_new
    inline void set(Node* p, Node* p_new) {
        if (p == p1)
            p1 = p_new;
        else
            p2 = p_new;
    }
};

Node* nodes_bank; // nodes pre-allocated
int n_used; // no of used nodes in nodes bank
int n; // total current number of nodes

Node* header; // p1 is always right
Node* tailer; // p2 is always left

char* buffer; // output buffer
int len; // output buffer length

/*
The cursor
*/
struct Cursor {
    Node* p_right;
    Node* p_left;
    int pos;
    
    // Copy cursor
    inline void copy(Cursor& c) {
        p_right = c.p_right;
        p_left = c.p_left;
        pos = c.pos;
    }

    // Move the cursor
    // dir: left(<0), right(>=0)
    inline char move(int dir) {
        if (dir < 0) {
            if (pos <= 0) // at leftmost pos
                return 'F';
            else {
                pos -= 1;
                Node* p_left_left = p_left->other(p_right);
                p_right = p_left;
                p_left = p_left_left;
                return 'T';
            }
        } else {
            if (pos >= n){ // at rightmost pos
                return 'F';
            }
            else {
                pos += 1;
                Node* p_right_right = p_right->other(p_left);
                p_left = p_right;
                p_right = p_right_right;
                return 'T';
            }
        }
    }

    // Insert
    inline char insert(char c) {
        // get new node
        Node* p = nodes_bank + n_used;
        n_used++;
        p_left->set(p_right, p); // make left node point to p
        p_right->set(p_left, p); // make right node point to p

        // init p
        p->c = c;
        p->p1 = p_left;
        p->p2 = p_right;

        // update cursor
        p_left = p;
        pos++;
        n++;
        return 'T';
    }

    // Delete
    inline char del() {
        if (pos >= n)
            return 'F';
        Node* p_right_right = p_right->other(p_left);
        // make the left node point to the node beyond p_right
        p_left->set(p_right, p_right_right);
        // and vice versa
        p_right_right->set(p_right, p_left);
        // update cursor
        p_right = p_right_right;
        n--;
        return 'T';
    }
};

inline void show() {
    Node* p_prev = header;
    Node* p_cur = header->p1;
    while(p_cur != tailer) {
        // append to buffer
        buffer[len++] = p_cur->c;
        // move the pointer
        Node* tmp = p_cur;
        p_cur = p_cur->other(p_prev);
        p_prev = tmp;
    }
}

inline char reverse(Cursor& c_l, Cursor& c_r) {
    if (c_l.pos >= c_r.pos)
        return 'F';
    
    // point cursor to correct locations
    Node* tmp = c_l.p_right;
    c_l.p_right = c_r.p_left;
    c_r.p_left = tmp;
    
    // "reverse" the section between c_l and c_r
    c_r.p_left->set(c_l.p_left, c_r.p_right);
    c_l.p_right->set(c_r.p_right, c_l.p_left);

    // re-connect the reversed section
    c_l.p_left->set(c_r.p_left, c_l.p_right);
    c_r.p_right->set(c_l.p_right, c_r.p_left);

    return 'T';
}

int main() {
    // perform memory allocations
    // and init variables
    header = new Node();
    tailer = new Node();
    header->p1 = tailer;
    header->p2 = 0;
    tailer->p1 = 0;
    tailer->p2 = header;

    nodes_bank = new Node[8500000];
    buffer = new char[30000000];

    Cursor cursor_l, cursor_r;

    // initial string
    char c = getchar();
    while (c != '\n') {
        Node* p = nodes_bank + n_used;
        n_used++;
        n++;
        p->c = c;
        p->p1 = tailer;
        p->p2 = tailer->p2;
        tailer->p2 = p;
        p->p2->set(tailer, p);
        c = getchar();
    }
    cursor_l.pos = 0;
    cursor_l.p_left = header;
    cursor_l.p_right = header->p1;
    cursor_r.pos = n;
    cursor_r.p_left = tailer->p2;
    cursor_r.p_right = tailer;

    // no. of instructions
    int n_instructions;
    scanf("%d", &n_instructions);
    c = getchar();
    while(c != '\n')
        c = getchar();

    // process instructions
    char *buf;
    size_t size = 10;
    buf = new char[size];
    for(int i = 0; i < n_instructions; i++) {
        getline(&buf, &size, stdin);
        // printf("%s", buf);
        switch (buf[0])
        {
        case '>':{
            if (buf[2] == 'L') {
                buffer[len++] = cursor_l.move(1);
            } else {
                // buf[2] == 'R'
                buffer[len++] = cursor_r.move(1);
            }
            break;
        }
        case '<':
            if (buf[2] == 'L') {
                buffer[len++] = cursor_l.move(-1);
            } else {
                buffer[len++] = cursor_r.move(-1);
            }
            break;
        case 'I':
            if (buf[2] == 'L') {
                buffer[len++] = cursor_l.insert(buf[4]);
                // side effects
                if (cursor_r.pos + 1 == cursor_l.pos) {
                    // c_r overlaps with c_l before insertion
                    cursor_r.copy(cursor_l);
                } else if (cursor_r.pos >= cursor_l.pos) {
                    // c_r right of c_l
                    cursor_r.pos++;
                }
            } else {
                buffer[len++] = cursor_r.insert(buf[4]);
                if (cursor_l.pos + 1 == cursor_r.pos) {
                    // overlap before insertion
                    cursor_l.copy(cursor_r);
                } else if (cursor_l.pos >= cursor_r.pos) {
                    // c_l right of c_r
                    cursor_l.pos++;
                }
            }
            break;
        case 'R':
            buffer[len++] = reverse(cursor_l, cursor_r);
            break;
        case 'D':
            if (buf[2] == 'L') {
                buffer[len++] = cursor_l.del();
                if (buffer[len-1] == 'T') {
                    if (cursor_r.pos == cursor_l.pos) {
                        // overlap after deletion
                        cursor_r.copy(cursor_l);
                    } else if (cursor_r.pos == cursor_l.pos + 1) {
                        // overlap before deletion
                        cursor_r.copy(cursor_l);
                    } else if (cursor_r.pos > cursor_l.pos + 1) {
                        // c_r right of c_l
                        cursor_r.pos--;
                    }
                }
            } else {
                buffer[len++] = cursor_r.del();
                if (buffer[len-1] == 'T') {
                    if (cursor_l.pos == cursor_r.pos) {
                        // overlap after deletion
                        cursor_l.copy(cursor_r);
                    } else if (cursor_l.pos == cursor_r.pos + 1) {
                        // overlap before deletion
                        cursor_l.copy(cursor_r);
                    } else if (cursor_l.pos > cursor_r.pos + 1) {
                        // c_l right of c_r
                        cursor_l.pos--;
                    }
                }
            }
            break;
        case 'S':
            show();
            break;
        }
        buffer[len++] = '\n';
    }

    buffer[len++] = 0;
    printf("%s", buffer);
    return 0;
}