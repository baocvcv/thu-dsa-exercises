

file = open("input.txt", 'r')
line = file.readline().split()
n = int(line[0])
k = int(line[1])
data = []
for i in range(3):
    line = file.readline().split()
    data.append([int(line[k]) for k in range(n)])

sums = []
for x in range(n):
    for y in range(n):
        for z in range(n):
            sums.append(data[0][x] + data[1][y] + data[2][z])
sums.sort()
print(sums)
print(sums[k-1])
