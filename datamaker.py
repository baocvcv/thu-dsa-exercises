#encoding=utf-8

import random
import sys

def dataMaker(n, valuelength):
	f = sys.stdout
	f.write(str(n) + "\n")
	for datanum in range(n):
		S1 = str(random.randint(1, 9))
		S2 = str(random.randint(1, 9))
		for itr in range(valuelength - 1):
			S1 += str(random.randint(0, 9))
			S2 += str(random.randint(0, 9))
		f.write(S1 + " " + S2 + "\n")


# 每组数据10个，长度为10
dataMaker(10, 10)
