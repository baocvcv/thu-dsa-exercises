#include <vector>
#include <cstdio>

using namespace std;

int main(){
    vector<char> vec;
    char c;
    c = getchar();
    while(c != '\n'){
        vec.push_back(c);
        c = getchar();
    }

    auto merge = [&]() -> bool {
        if(vec.size() < 3){
            return false;
        }
        vector<char>::iterator it_prev, it_cur;
        it_prev = vec.begin();
        bool flag = false;
        while(it_prev != vec.end()){
            it_cur = it_prev + 1;
            int count = 1;
            while(it_cur != vec.end() && *it_cur == *it_prev){
                // printf("%c %c %d\n", *it_prev, *it_cur, count);
                it_cur++;
                count++;
            }
            if(count >= 3){
                flag = true;
                for (; count > 0; count--){
                    it_prev = vec.erase(it_prev);
                }
            }else{
                it_prev = it_cur;
            }
        }
        return flag;
    };

    auto print = [&]() {
        for (int i = 0; i < vec.size(); i++)
        {
            putchar(vec[i]);
        }
        putchar('\n');
    };

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++){
        int x;
        char c;
        scanf("%d", &x);
        getchar();
        c = getchar();
        vec.insert(vec.begin() + x, c);

        // printf("%d %c\n", x, c);
        // print();

        while(merge()){
            // print();
        }
    }

    // printf("******");
    print();

    return 0;
}