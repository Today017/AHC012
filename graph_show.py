from matplotlib import pyplot as pyp

f = open('out.txt', 'r')

n = int(f.readline())
a = list(map(int, f.readline().split()))

pyp.plot(a)
pyp.show()