#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>
# define DEFAULT_CAPACITY 3
const int M=400040, N=100010;

int counte;

inline int min(int a, int b){
  return a<b?a:b;
}
inline int max(int a, int b){
  return a>b?a:b;
}

template <typename T> class Vector{
  private:
    int _size;
    int _capacity;
    T* _elem;
  protected:
    void copyFrom(T const* A, int lo, int hi){
      _elem = new T[_capacity = 2*(hi-lo)];
      _size = 0;
      while(lo<hi)
        _elem[_size++] = A[lo++];
    }
    void expand(){
      if(_size<_capacity)return;
      _capacity = max(_capacity, DEFAULT_CAPACITY);
      _capacity = _capacity<<1;
      T* oldElem = _elem; _elem = new T[_capacity];
      for(int i=0; i<_size; i++)
        _elem[i] = oldElem[i];
      delete[] oldElem;
    }

    void shrink(){
      if(_size + 1 > _capacity/2)
        return;
      _capacity = max(_capacity, DEFAULT_CAPACITY<<1);
      _capacity = _capacity>>1;
      T* oldElem = _elem; _elem = new T[_capacity];
      for(int i=0; i<_size; i++)
        _elem[i] = oldElem[i];
      delete[] oldElem;
    }

    /* Comparison function. Receives two generic (void) pointers to the items under comparison. */
    static int compare_ints(const void *p, const void *q) {
      int x = *(const int *)p;
      int y = *(const int *)q;

      /* Avoid return x - y, which can cause undefined behaviour
         because of signed integer overflow. */
      if (x < y)
        return -1;  // Return -1 if you want ascending, 1 if you want descending order.
      else if (x > y)
        return 1;   // Return 1 if you want ascending, -1 if you want descending order.

      return 0;
    }


  public:
    Vector(int c = DEFAULT_CAPACITY){
      _elem = new T[_capacity = c];
      _size = 0;
    }
    Vector(T const& A, int lo, int hi){
      copyFrom(A, lo, hi);
    }
    Vector(Vector<T> const& V, int lo, int hi){
      copyFrom(V._elem, lo, hi);
    }
    Vector(Vector<T> const& V){
      copyFrom(V._elem, 0, V._size);
    }
    ~Vector(){
      delete [] _elem;
    }

    int insert(int r, T const & e){
      expand();
      for(int i=_size; i>r; i--){
        _elem[i] = _elem[i-1];
      }
      _elem[r]=e;
      _size++;
      return r;
    }
    int remove(int lo, int hi){
      if(lo == hi) return 0;
      while(hi < _size)
        _elem[lo++] = _elem[hi++];
      _size = lo;
      shrink();
      return hi - lo;
    }
    T remove(int r){
      T e = _elem[r];
      remove(r, r+1);
      return e;
    }
    int push_back(T const& e){
      return insert(_size, e);
    }
    T& operator[](int r){
      return _elem[r];
    }
    int size(){
      return _size;
    }
    void sort(){
      qsort(_elem, _size, sizeof *_elem, compare_ints);
    }
};

int dfn[N];           //时间戳
int dfs_clock;           //时间计数器
bool is_cut[N];         //节点是否为割点
int dcc_cnt;             //双连通分量计数器
int new_id[N];         //缩点后原先节点新的编号
int old_id[N];
int stack[N];
int top;
Vector<int> dcc[N];
Vector<int> new_node[2*N];
Vector<int> nodes[N];

void adde(int u, int v){
  nodes[u].push_back(v);
  nodes[v].push_back(u);
}


int dfs(int u, int parent){
  int lowu;
  dfn[u] = lowu = ++dfs_clock;
  stack[++top] = u;
  if(!parent && !nodes[u].size())
  {
    dcc[++dcc_cnt].push_back(u);//cnt联通块标号
    return 0;
  }
  int flag = 0;
  for(int i=0; i<nodes[u].size(); i++){
    int v = nodes[u][i];
    if(!dfn[v]){
      int lowv = dfs(v, u);
      lowu = min(lowu, lowv);
      if(lowv >= dfn[u]){
        flag++;
        if(parent || flag>1)
          is_cut[u]=true;
        dcc_cnt++;
        int w;
        while(1){
          w=stack[top--];
          dcc[dcc_cnt].push_back(w);
          if(w == v)
            break;
        }
        dcc[dcc_cnt].push_back(u);
      }
    }
    else lowu = min(lowu, dfn[v]);
  }
  return lowu;
}

void search(int a, int b, Vector<int>& ans){
  int u = new_id[a];
  int v = new_id[b];
  if(u==v)
    return;
  int sch_stk[2*N];
  int path[2*N];
  bool visit[2*N];
  memset(visit, 0 ,sizeof(visit));
  int top = 1;
  sch_stk[0] = u;
  while(top>0){
    int c = sch_stk[--top];
    if(visit[c]){
      continue;
    }
    if(c == v){
      int cur=path[v];
      while(cur!=u){
        if(cur>dcc_cnt){
          ans.push_back(old_id[cur-dcc_cnt]);
        }
        cur = path[cur];
      }
      return;
    }
    visit[c] = 1;
    for(int i=0; i<new_node[c].size(); i++){
      if(!visit[new_node[c][i]]){
        sch_stk[top++] = new_node[c][i];
        path[new_node[c][i]] = c;
      }
    }
  }
  return;
}

int main(){
  int n, m;
  scanf("%d%d", &n, &m);
  for(int i=0; i<m; i++){
    int u, v;
    scanf("%d%d", &u, &v);
    adde(u, v);
    adde(v, u);
  }
  dfs(1, 0);
  int new_cnt = dcc_cnt;
  for(int i=1; i<=dcc_cnt; i++)
  {
    for(int j=0; j<dcc[i].size(); ++j){
      int id = dcc[i][j];
      if(is_cut[id]){
        if(!new_id[id]){
          new_cnt++;
          new_id[id] = new_cnt;
          old_id[new_cnt - dcc_cnt] = id;
        }
        new_node[new_id[id]].push_back(i);
        new_node[i].push_back(new_id[id]);
      }
      else{
        new_id[id] = i;
      }
    }
  }
  int q;
  scanf("%d", &q);
  for(int i=0; i<q; i++){
    int u, v;
    scanf("%d%d", &u, &v);
    Vector<int> ans;
    ans.push_back(u);
    if(u!=v)
      ans.push_back(v);
    search(u, v, ans);
    ans.sort();
    for(int j=0; j<ans.size(); j++){
      printf("%d ", ans[j]);
    }
    printf("\n");
  }
  return 0;
}
