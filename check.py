#encoding=utf-8

import os
import sys

def checkAns(outputFile1, outputFile2):
	"""
		该函数用于判断两个输入文件是否相同，这里采用的方式是读入两个文件的所有行，然后拼起来判断是否相同。可以根据需求修改
		outputFile1,outputFile2 分别为判断的两个文件的文件名
		函数返回布尔值
	"""
	with open(outputFile1, "r") as f:
		output1 = f.readlines()
	with open(outputFile2, "r") as f:
		output2 = f.readlines()
	return "".join(output1) == "".join(output2)

if len(sys.argv) - 1 != 3:
	sys.stdout.write("Usage: %s <program1> <program2> <datamaker>\n" % sys.argv[0])
	sys.stdout.write("    Example on Linux: python3 %s ./a \"python3 b.py\" \"python3 datamaker.py\"\n" % sys.argv[0])
	sys.stdout.write("    Example on Windows: python3 %s a.exe \"python3 b.py\" \"python3 datamaker.py\"\n" % sys.argv[0])
	exit(1)

cnt = 0
while True:
	cnt += 1
	sys.stdout.write("Running Case #%s ... " % cnt)
	if 0 != os.system("%s > input.txt" % sys.argv[3]):
		sys.stdout.write("Failed\n")
		break
	if 0 != os.system("%s < input.txt > output1.txt" % sys.argv[1]):
		sys.stdout.write("Failed\n")
		break
	if 0 != os.system("%s < input.txt > output2.txt" % sys.argv[2]):
		sys.stdout.write("Failed\n")
		break
	if not checkAns("output1.txt", "output2.txt"):
		sys.stdout.write("Wrong\n")
		break
	sys.stdout.write("OK\n")
