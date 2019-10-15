#encoding=utf-8

import random
import sys

INITIAL_STRING = """ABCDABCDABCDABCDABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ"""

def dataMaker(n):
	f = sys.stdout
	f.write(INITIAL_STRING + "\n")
	f.write(str(n) + "\n")
	for datanum in range(n):
		S1 = str(random.randint(0, 21))
		S2 = chr(random.randint(65, 70))
		f.write(S1 + " " + S2 + "\n")

# 每组数据10个，长度为10
dataMaker(5000)
