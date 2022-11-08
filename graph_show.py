from matplotlib import pyplot as pyp
n = int(input())
a = list(map(int, input().split()))

pyp.plot(a)
pyp.show()