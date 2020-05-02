import pycosat
import random

a = 1.0
k = 3
n = 40
T = 150
f = open("dane.txt", "w")


def getRandVar():
    sign = [1, -1]
    variable = range(1, n + 1)
    x = random.choice(variable) * random.choice(sign)
    return x


while a <= 10.0:
    S = 0
    for p in range(0, T):
        cnf = []
        for q in range(0, int(a*n)):
            clause = []
            for r in range(0, k):
                clause.append(getRandVar())
            cnf.append(clause)
        if pycosat.solve(cnf) != 'UNSAT':
            S = S + 1
    f.write(a.__str__() + " " + (S / T).__str__() + "\n")
    a = round(a + 0.1, 1)
print("done")
