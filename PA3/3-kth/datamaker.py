#encoding=utf-8

import random
import sys

def dataMaker(n):
	f = sys.stdout
	f.write(str(n) + " " + str(random.randint(1, n*n*n)) + "\n")
	m = n * n * 5
	for _ in range(3):
		for _ in range(n):
			f.write(str(random.randint(1, m)) + ' ')
		f.write("\n")


# 每组数据10个，长度为10
dataMaker(3)
