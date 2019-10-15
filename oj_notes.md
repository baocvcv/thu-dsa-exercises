## Report guidlines
- 所使用数据结构与算法的构思、原理和实现要点。
- 完成过程中遇到的问题，排除问题的主要过程、使用的方法和技巧，以及参考资料。
- 时间和空间复杂度的估算。
- （可选）介绍理论分析与实测效果的吻合程度，不吻合时进一步解释原因。
- （可选）所用方法的特别、新颖或创新之处。

## Tricks
### I/O buffer
```c++
setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
```

### I/O redirection
```c++
#ifndef _OJ_
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
#endif
```