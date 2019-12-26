#encoding=utf-8

import random
import string
import sys

def dataMaker(n):
	f = sys.stdout
	letters = "abcdefghijklmnopqrstuvwxyz"
	for _ in range(n):
		f.write(random.choice(letters))
	f.write('\n')

# 每组数据10个，长度为10
dataMaker(1000)
