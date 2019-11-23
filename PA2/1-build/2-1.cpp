#define MAXN 1000010

#include <iostream>

int N, M;

//对长子兄弟法进行微调，在其基础上增加最后一个孩子
//并增加大小、高度、当前节点在父节点的的孩子序列中从后往前的最大高度三个参数
//其中当前节点在父节点的的孩子序列中从后往前的最大高度指从这个孩子节点到最后一个孩子节点的区间内的最大高度
struct node
{
    int parent, last = -1, next = 0;
    int first_child = -1, last_child = 0;
    int id, size = 1, height = 0, cur_height = 0;
};

//存储树的节点
node nodes[MAXN];

//用于后序遍历的辅助栈
int stack[MAXN], top = 0;

//用于记录节点是否进过栈，只有没有进过栈的节点需要进栈
bool visit[MAXN];

//后序遍历将需要访问的节点添加到栈中
void gotoHLVFL(){
    int x = stack[top];
    while(nodes[x].first_child != -1){
        int tmp = nodes[x].first_child;
        while(tmp != 0){
            if(!visit[tmp])//如果其曾经在栈中出现过则不推入栈中，否则推入栈中
                stack[++top] = tmp;
            tmp = nodes[tmp].next;
        }
        if(x == stack[top])break;
        x = stack[top];
    }
}

//使用栈进行后序遍历初始化每个子树的规模
void init_size(){
    //将栈和访问信息初始化
    for(int i = 0; i < MAXN; i++)visit[i] = 0;
    top = 0;
    stack[++top] = 1;

    gotoHLVFL();
    //循环直到栈空，栈空即说明全部访问完成
    while(top > 0){
        //对节点进行访问
        visit[stack[top]] = 1;
        //如果该节点有孩子，则访问其所有孩子，将所有子树的规模相加再加上自己即为规模
        if(nodes[stack[top]].first_child != -1){
            int size = 1, tmp = nodes[stack[top]].last_child;
            while(tmp != -1){
                size += nodes[tmp].size;
                tmp = nodes[tmp].last;
            }
            nodes[stack[top]].size = size;
        }
        top--;
        gotoHLVFL();
    }
}

//使用栈进行后序遍历初始化每个子树的高度
void init_height(){
    //将栈和访问信息初始化
    for(int i = 0; i < MAXN; i++)visit[i] = 0;
    top = 0; 
    stack[++top] = 1;
    
    gotoHLVFL();
    //循环直到栈空，栈空即说明全部访问完成
    while(top > 0){
        //对节点进行访问
        visit[stack[top]] = 1;
        //如果该节点有孩子，则访问其所有孩子
        //通过维护当前该孩子后所有节点的最大高度来更新每个孩子节点的最大高度的信息
        if(nodes[stack[top]].first_child != -1){
            int max_height = 0, tmp = nodes[stack[top]].last_child;
            while(tmp != -1){
                //更新最大高度的信息
                max_height = max_height > nodes[tmp].height ? max_height : nodes[tmp].height;
                //更新每个孩子节点的最大高度
                nodes[tmp].cur_height = max_height;
                tmp = nodes[tmp].last;
            }
            //每个有孩子的节点的高度即为其第一个孩子的最大高度加一
            nodes[stack[top]].height = max_height + 1;
        }
        top--;
        gotoHLVFL();
    }
}

//返回路径上最后一个合法节点的序号
int valid_id(int num){
    int cur = 1, pos;//分别为指向当前节点的指正和记录是第几个孩子
    int last_round = cur;//记录当前查找下的最后一个合法节点
    bool flag = 1;//记录路径是否已经合法
    for(int i = 0; i < num; i++){
        scanf("%d", &pos);
        if(flag){
            if(nodes[cur].first_child != -1)
                cur = nodes[cur].first_child;
            else
                {flag = 0; continue;}
            while(pos--){
                if(nodes[cur].next != 0)
                    cur = nodes[cur].next;
                else
                    {flag = 0; break;}
            }
            //如果合法则更新最后一个合法节点，否则不更新
            if(flag)
                last_round = cur;
        }
    }
    return last_round;
}

//更新节点高度
int update(int id){
    //如果更新到根节点，则不需要再向上更新
    if(id == 1){nodes[1].cur_height = nodes[1].height = nodes[nodes[1].first_child].cur_height + 1; return -1;}
    int parent = nodes[id].parent;
    int old_parent_height = nodes[parent].cur_height;
    //如果该节点是父节点的最后一个孩子
    if(nodes[id].next == 0)
        nodes[id].cur_height = nodes[id].height;
    //否则根据后面节点的信息更新高度
    else
        nodes[id].cur_height = nodes[id].height > nodes[nodes[id].next].cur_height ?
                                nodes[id].height : nodes[nodes[id].next].cur_height;
    int cur = nodes[id].last;
    //向前更新高度，直到到达父亲的第一个孩子
    while(cur != -1){
        nodes[cur].cur_height = nodes[cur].height > nodes[nodes[cur].next].cur_height ?
                                nodes[cur].height : nodes[nodes[cur].next].cur_height;
        cur = nodes[cur].last;
    }

    //更新父亲的高度
    nodes[parent].height = nodes[nodes[parent].first_child].cur_height + 1;

    //更新父亲在其父亲的孩子序列中的最大高度信息
    if(nodes[parent].next == 0)
        nodes[parent].cur_height = nodes[parent].height;
    else
        nodes[parent].cur_height = nodes[parent].height > nodes[nodes[parent].next].cur_height ?
                                nodes[parent].height : nodes[nodes[parent].next].cur_height;
    //如果父亲节点的最大高度没有更新，则不需要再向上更新    
    if(nodes[parent].cur_height == old_parent_height)
        return -1;
    //如果父亲节点的最大高度更新了，则需要继续更新
    else
        return parent;
}

//移除子树
void remove(int id){
    int parent = nodes[id].parent;
    int tmp = parent;
    int pos = nodes[id].last;
    
    //向上更新祖先的规模
    nodes[id].parent = 0;
    nodes[parent].size -= nodes[id].size;
    nodes[id].cur_height = nodes[id].height;
    while(parent != 1){
        parent = nodes[parent].parent;
        nodes[parent].size -= nodes[id].size;
    }
    parent = tmp;

    //对于子树在父亲节点中不同位置的情况分类讨论

    //如果是唯一的孩子，更新父亲的高度
    if(nodes[id].last == -1 && nodes[id].next == 0){
        nodes[parent].height = 0;
        nodes[parent].first_child = -1;
        nodes[parent].last_child = 0;
    }
    //如果是第一个孩子，更新父亲的高度
    else if(nodes[id].last == -1){
        nodes[parent].first_child = nodes[id].next;
        nodes[nodes[id].next].last = -1;
        nodes[id].next = 0;
        nodes[parent].height = nodes[nodes[parent].first_child].cur_height + 1;
    }
    //如果是第最后一个孩子
    else if(nodes[id].next == 0){
        nodes[parent].last_child = nodes[id].last;
        nodes[nodes[id].last].next = 0;
        nodes[id].last = -1;
    }
    //如果是中间的孩子
    else{
        int tmp_last_child = nodes[id].last, tmp_next_child = nodes[id].next;
        nodes[tmp_last_child].next = tmp_next_child;
        nodes[tmp_next_child].last = tmp_last_child;
        nodes[id].last = -1; nodes[id].next = 0;
    }
    //如果不是第一个孩子，更新孩子及祖先的高度直到某个祖先的最大高度不变
    if(pos != -1){
        int tmp_update = update(pos);
        while(tmp_update != -1)tmp_update = update(tmp_update);
    }
    //如果是第一个孩子，由于上面的分类讨论已经经过预处理，直接更新其父亲及祖先即可
    else{
        int tmp_update = update(parent);
        while(tmp_update != -1)tmp_update = update(tmp_update);
    }
}

//将子树插入到节点中
void insert(int des, int rank, int id){
    int obj = nodes[des].first_child;
    int parent = des;
    int tmp = parent;

    //向上更新祖先的规模
    nodes[id].parent = 0;
    nodes[parent].size += nodes[id].size;
    while(parent != 1){
        parent = nodes[parent].parent;
        nodes[parent].size += nodes[id].size;
    }
    parent = tmp;

    //找到需要插入的节点位置，在其前面插入
    while(rank-- && obj != 0 && obj != -1)obj = nodes[obj].next;

    //对于子树在父亲节点中不同位置的情况分类讨论
   
    nodes[id].parent = parent;
    //如果是唯一的孩子
    if(nodes[parent].first_child == -1 && nodes[parent].last_child == 0){
        nodes[parent].first_child = id;
        nodes[parent].last_child = id;
        nodes[id].last = -1; nodes[id].next = 0;
        nodes[parent].height = nodes[id].height + 1;
    }
    //如果是第一个孩子
    else if(obj == nodes[parent].first_child){
        int tmp_first_child = nodes[parent].first_child;
        nodes[parent].first_child = id;
        nodes[id].last = -1; nodes[id].next = tmp_first_child;
        nodes[tmp_first_child].last = id;
    }
    //如果是第最后一个孩子
    else if(obj == 0){
        int tmp_last_child = nodes[parent].last_child;
        nodes[parent].last_child = id;
        nodes[id].last = tmp_last_child; nodes[id].next = 0;
        nodes[tmp_last_child].next = id;
    }
    //如果是中间的孩子
    else{
        int tmp_last_child = nodes[obj].last, tmp_next_child = obj;
        nodes[tmp_last_child].next = id;
        nodes[tmp_next_child].last = id;
        nodes[id].last = tmp_last_child; nodes[id].next = tmp_next_child;
    }

    //由于插入后父节点下一定有节点，所以必须更新高度
    int tmp_update = update(id);
    while(tmp_update != -1)tmp_update = update(tmp_update);
}

int main(int argc, char const *argv[])
{
    for(int i = 0; i < MAXN; i++)nodes[i].id = i;
    scanf("%d%d", &N, &M);
    //将树读入
    for(int i = 0; i < N; i++){
        int num, child_id; scanf("%d", &num);
        int old = -1;
        if(num > 0){
            scanf("%d", &child_id);
            nodes[i + 1].first_child = child_id;
            nodes[child_id].parent = i + 1;
            old = child_id;
        }
        for(int j = 1; j < num; j++){
            scanf("%d", &child_id);
            nodes[child_id].last = old;
            nodes[child_id].parent = i + 1;
            nodes[old].next = child_id;
            old = child_id;
        }
        if(num > 0)nodes[i + 1].last_child = child_id;
    }

    //初始化树的大小和高度
    init_size();
    init_height();

    int comm;
    int sour, dest, rank, refer;
    int sour_id, dest_id, rank_id, refer_id;

    //执行操作
    for(int i = 0; i < M; i++){
        scanf("%d", &comm);
        if(comm == 0){
            scanf("%d", &sour);
            sour_id = valid_id(sour);
            remove(sour_id);
            scanf("%d", &dest);
            dest_id = valid_id(dest);
            scanf("%d", &rank);
            insert(dest_id, rank, sour_id);
        }
        else if(comm == 1){
            scanf("%d", &refer);
            printf("%d\n", nodes[valid_id(refer)].height);
        }
        else{
            scanf("%d", &refer);
            printf("%d\n", nodes[valid_id(refer)].size);
        }
    }

    return 0;
}