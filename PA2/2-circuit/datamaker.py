#encoding=utf-8

import random
import sys


def dataMaker(n, k, m):
	f = sys.stdout
	f.write("%d %d\n" % (n * m, k))
	l = []
	for _ in range(n):
		S1 = str(random.randint(0, 1))
		for _ in range(63):
			S1 += str(random.randint(0, 1))
		l.append(S1)

	for _ in range(m * n):
		i = random.randint(0, n-1)
		f.write(l[i] + '\n')

# 每组数据10个，长度为10
dataMaker(12, 140, 12)
