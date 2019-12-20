#include <cstdio>

char mat[1000][1000];
int n, m;
int res;

int dx[] = { -1, 1, 0, 0};
int dy[] = {  0, 0,-1, 1};

struct Point
{
    int x, y;

    Point () {}
    Point (int _x, int _y): x(_x), y(_y) {}
};

Point queue[1000005];
int size;

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%s", &mat[i]);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mat[i][j] == '0') {
                queue[size++] = Point(i, j);
            }
        }
    }

    int time = 1;
    int i = 0;
    int old_size = size;
    while (size < n * m) { // bfs
        for (; i < old_size; i++) {
            for (int j = 0; j < 4; j++) {
                int x = queue[i].x + dx[j];
                int y = queue[i].y + dy[j];
                if (x >= 0 && x < n && y >= 0 && y < m && mat[x][y] == '1') {
                    mat[x][y] = '0';
                    res += time;
                    queue[size++] = Point(x, y);
                }
            }
        }
        old_size = size;
        time++;
    }
    printf("%d\n", res);
    return 0;
}