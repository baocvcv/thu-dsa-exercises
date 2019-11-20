#encoding=utf-8

import random
import sys

N = 100
M = 10

def dataMaker():
	f = sys.stdout
	f.write("%d %d\n" % (N, M))
	for _ in range(M):
		c = random.randint(0, 2)
		if c == 0 :
			f.write("Q ")
		else:
			f.write("H ")

		S1 = random.randint(1, N)
		S2 = random.randint(S1, N)
		f.write("%d %d\n" % (S1, S2))

# 每组数据10个，长度为10
dataMaker()
