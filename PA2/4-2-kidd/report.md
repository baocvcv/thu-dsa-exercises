## PA4-2 Kidd
2018011289 包涵

## 使用的数据结构与算法
- 标准线段树
- 将待查询点自底向上建树，再将每次翻转操作对应记录在节点上即可

## 遇到的问题与解决方法
- pushdown一开始没有考虑清楚，后来发现不需要pushdown，查询的时候沿途加起来就可以了

## 复杂度估计
- 时间复杂度: $O(mlog(m))$
  - 树高最多$log(m)$，每次翻转和查询，询问点数都不超过两倍树高
- 空间复杂度： $O(m)$