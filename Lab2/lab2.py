from dimacs import *
from random import randint

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
    ("k330_d"),
    ("k330_e"),
    ("k330_f"),
    ("f30"),
    ("f35"),
    ("f40"),
    # ("f56"),
    ("m20"),
    ("m30"),
    ("m40"),
    ("m50"),
    ("m100"),
    ("p20"),
    ("p35"),
    ("p60"),
    ("p150"),
    ("p200"),
    ("r30_01"),
    ("r30_05"),
    ("r50_001"),
    ("r50_01"),
    ("r50_05"),
    ("r100_005"),
    ("r100_01"),
    ("r200_001"),
    ("r200_005")
]

for name in graphs:
    name = "graph/" + name
    Graph = loadGraph(name)
    V = len(Graph)


    def getVertexWithMaxDeg(G, V):
        resultVertex = -1
        maxDeg = 0
        for v in range(0, V):
            deg = len(G[v])
            if deg > maxDeg:
                resultVertex = v
                maxDeg = deg
        return resultVertex


    def isEmpty(G):
        for s in G:
            if s != set():
                return True
        return False

    # aproksymacja log(n)
    def approximationGreedy(G, S):
        if isEmpty(G) == 0:
            return S
        v = getVertexWithMaxDeg(G, V)
        for u in G[v].copy():
            G[u].remove(v)
            G[v].remove(u)
        return approximationGreedy(G, S + (v,))


    # 2-aproksymacja
    def approximation2(G, S):
        if isEmpty(G) == 0:
            return S
        v = 0
        while G[v] == set():
            v = v + 1
            if v >= V:
                return ()
        u = G[v].pop()
        G[v].add(u)
        for x in G[v].copy():
            G[x].remove(v)
            G[v].remove(x)
        for x in G[u].copy():
            G[x].remove(u)
            G[u].remove(x)
        return approximation2(G, S + (u, v))


    S = ()
    C = ()
    if name == "graph/f56" or name == "graph/b100":
        C = approximation2(Graph.copy(), S)
    else:
        C = approximationGreedy(Graph.copy(), S)
    saveSolution(name + ".sol", C)
    print(name + " |solution| " + C.__str__())
