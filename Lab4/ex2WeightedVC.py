from dimacs import *
from pulp import *
import functools
import time

graphs = [
    ("e5"),
    ("e10"),
    ("e20"),
    ("e40"),
    ("e150"),
    ("s25"),
    ("s50"),
    ("s500"),
    ("b20"),
    ("b30"),
    ("b100"),
    ("m20"),
    ("m30"),
    ("m40"),
    ("m50"),
    ("m100"),
    ("p20"),
    ("p35"),
    ("p60"),
    ("p150"),
    ("r30_01"),
    ("r50_001"),
    ("r50_01"),
    ("r100_005"),
]
start = time.time()
for name in graphs:
    name = "graph/" + name
    G = loadGraph(name)
    V = len(G)
    edges = edgeList(G)


    def getListOfDegs(G, V):
        degs = []
        for v in range(1, V):
            deg = 0
            for (x, y) in G:
                if x == v or y == v:
                    deg = deg + 1
            degs.append(deg)
        return degs


    def ilpSolution(G):
        model = LpProblem("Vertex_Cover", LpMinimize)
        x = []
        y = getListOfDegs(G, V)
        print(y)
        z = []
        for i in range(1, V):
            var = LpVariable("x" + str(i), cat="Binary")
            x.append(var)
            z.append(var*y[i-1])

        model += functools.reduce(lambda a, b: a + b, z)
        for edge in G:
            model += x[edge[0]-1] + x[edge[1]-1] >= 1
        model.solve()
        S = ()
        for i in range(1, V):
            if x[i-1].value() == 1.0:
                S += (i,)
        return S


    S = ()
    C = ilpSolution(edges)
    saveSolution(name + ".sol", C)
    print(name + " |solution| " + C.__str__())

end = time.time()
print(end-start)


# TIME : 32.69 s
# SCORE (grademe.py) : 1334
