#encoding=utf-8

import random
import sys


def dataMaker(n, m, q):
    f = sys.stdout
    f.write(str(n) + " " + str(m) + "\n")
    for datanum in range(m):
        e = random.sample(range(1, n), 2)
        f.write(str(e[0]) + " " + str(e[1]) + "\n")
    f.write(str(q) + "\n")
    for datanum in range(m):
        e = random.sample(range(1, n), 2)
        f.write(str(e[0]) + " " + str(e[1]) + "\n")


# 每组数据10个，长度为10
dataMaker(1000, 5000, 10)
