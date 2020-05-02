from pulp import *


def findSolution(x, y):
    model = LpProblem("ex1", LpMinimize)
    model += x + y

    model += y >= x - 1
    model += y >= -4 * x + 4
    model += y <= -0.5 * x + 3

    print(model)

    model.solve()
    print(LpStatus[model.status])

    for var in model.variables():
        print(var.name, "=", var.varValue)

    print("minimal objective:", value(model.objective))


x = LpVariable("x", cat="Integer")
y = LpVariable("y", cat="Integer")

findSolution(x, y)
print("--------------")

x = LpVariable("x", cat="Continuous")
y = LpVariable("y", cat="Continuous")

findSolution(x, y)
