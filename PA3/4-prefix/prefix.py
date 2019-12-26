file = open('input.txt', 'r')

text = file.readline().strip()

total = 0
for i in range(len(text)):
    j = 0
    while i + j < len(text) and text[j] == text[i + j]:
        j += 1
    # print(j)
    total += j

print(total)