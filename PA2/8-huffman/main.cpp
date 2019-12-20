#include <cstdio>
#include <cstdlib>

struct Node {
    int weight;
    char c;
    Node *l, *r;

    Node() {}
    Node (int _w, char _c):
        weight(_w), c(_c),
        l(NULL), r(NULL) {}
};

int bucket[26];

Node* merge(Node* n1, Node* n2) {
    int weight = n1->weight + n2->weight;
    Node* n = new Node(weight, 0);
    n->l = n1;
    n->r = n2;
    return n;
}

int sum;
void dfs_length(Node *n, int depth) {
    if (n->l == NULL && n->r == NULL) {
        sum += n->weight * depth; // sum
        return;
    }
    // go left
    dfs_length(n->l, depth + 1);
    // go right
    dfs_length(n->r, depth + 1);
}

char res[105];
int len;
void dfs_code(Node *n) {
    if (n->l == NULL && n->r == NULL) {
        // output
        res[len++] = '\0';
        printf("%c %s\n", n->c, res);
        len--;
        return;
    }

    // go left
    res[len++] = '0';
    dfs_code(n->l);
    // go right
    res[len-1] = '1';
    dfs_code(n->r);
    len--;
}

int main() {
    char s[500005];
    scanf("%s", s);
    int n;
    for (n = 0; s[n] != '\0'; n++) {
        // count
        bucket[s[n] - 'a']++;
    }

    // push initial nodes to stack
    Node* stack[100];
    int top;
    for (int i = 0; i < 26; i++) {
        if (bucket[i] > 0)
            stack[top++] = new Node(bucket[i], 'a' + i);
    }

    // huffman
    auto cmp = [](const void *n1, const void *n2) {
        return (*(Node**)n2)->weight - (*(Node**)n1)->weight;
    };
    while (top > 1) { // there should be one last element in the stack
        // sort the list by weight, from biggest to smallest
        qsort(stack, top, sizeof(Node*), cmp);
        // select the two smallest nodes and merge
        Node* n = merge(stack[top - 1], stack[top - 2]);
        top--;
        stack[top - 1] = n;
    }

    // dfs and print the length
    dfs_length(stack[0], 0);
    printf("%d\n", sum);

    // dfs and output the code
    dfs_code(stack[0]);

    return 0;
}