#encoding=utf-8

import random
import sys

def dataMaker(n, limit):
	f = sys.stdout
	f.write(str(n) + " " + str(limit) + "\n")
	num_list = [x for x in range(n)]
	random.shuffle(num_list)
	for x in num_list:
		f.write(str(x) + " ")
	f.write("\n")

# 每组数据10个，长度为10
dataMaker(1000000, 10000000)
