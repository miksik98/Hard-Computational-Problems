from dimacs import *
from pulp import *
import functools

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
    ("k330_a"),
    ("k330_b"),
    ("k330_c"),
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
    ("r30_05"),
    ("r50_001"),
    ("r50_01"),
    ("r50_05"),
    ("r100_005"),
]

for name in graphs:
    name = "graph/" + name
    G = loadGraph(name)
    V = len(G)
    edges = edgeList(G)


    def ilpSolution(G):
        model = LpProblem("Vertex_Cover", LpMinimize)
        x = []
        for i in range(0, V):
            x.append(LpVariable("x" + str(i + 1), cat="Binary"))
        model += functools.reduce(lambda a, b: a + b, x)
        for edge in G:
            model += x[edge[0]-1] + x[edge[1]-1] >= 1
        model.solve()
        S = ()
        for i in range(0, V):
            if x[i].value() == 1.0:
                S += (i+1,)
        return S


    S = ()
    C = ilpSolution(edges)
    saveSolution(name + ".sol", C)
    print(name + " |solution| " + C.__str__())
