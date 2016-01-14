import random

for i in range(100):
	a = random.random()
	b = random.random()
	print(a, b, ((a > 0.5) ^ (b > 0.5))*1)
