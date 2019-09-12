#encoding=utf-8

import random
import sys

def dataMaker(n, valuelength):
	f = sys.stdout
	f.write(str(n) + "\n")
	s1 = ""
	s2 = ""
	for datanum in range(n):
		v1 = str(random.randint(1, 9))
		v2 = str(random.randint(1, 9))
		for itr in range(valuelength - 1):
			v1 += str(random.randint(0, 9))
			v2 += str(random.randint(0, 9))
		s1 += v1 + " "
		s2 += v2 + " "
	f.write(s1 + "\n")
	f.write(s2 + "\n")

# 每组数据10个，长度为10
dataMaker(200000, 10)
dataMaker(200000, 10)
