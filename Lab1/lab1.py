from dimacs import *
from itertools import *

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


    def bruteForce(G, k):
        for C in combinations(range(V), k):
            if isVC(edgeList(G), C):
                return C
        return ()


    # 2^k
    def firstSolution(G, k, S):
        e = ()
        for (u, v) in G:
            if u not in S and v not in S:
                e = (u, v)
                break
        if e == ():
            return S
        if k == 0:
            return ()
        G0 = G.copy()
        for (u, v) in G0:
            if u == e[0] or v == e[0]:
                G0.remove((u, v))
        S0 = firstSolution(G0, k - 1, S + (e[0],))
        G1 = G.copy()
        for (u, v) in G1:
            if u == e[1] or v == e[1]:
                G1.remove((u, v))
        S1 = firstSolution(G1, k - 1, S + (e[1],))
        if S1 != ():
            return S0
        else:
            return S1


    def secondSolution(G, k, S):
        if k < 0:
            return ()
        if len(G) == 0:
            return S
        if k == 0:
            return ()
        u = G[0][0]

        neighbours = ()
        GMinusU = G.copy()
        for (x, y) in G:
            if x == u:
                neighbours = neighbours + (y,)
                GMinusU.remove((x, y))
            if y == u:
                neighbours = neighbours + (x,)
                GMinusU.remove((x, y))
        S0 = secondSolution(GMinusU, k - 1, S + (u,))

        GMinusNeigh = G.copy()
        for n in neighbours:
            for (x, y) in GMinusNeigh.copy():
                if x == n or y == n:
                    GMinusNeigh.remove((x, y))
        S1 = secondSolution(GMinusNeigh, k - len(neighbours), S + neighbours)

        if S0 != ():
            return S0
        else:
            return S1


    def getListOfDegs(G, V):
        degs = []
        for v in range(0, V):
            deg = 0
            for (x, y) in G:
                if x == v or y == v:
                    deg = deg + 1
            degs.append(deg)
        return degs


    def thirdSolution(G, k, S):
        if k < 0:
            return ()
        if len(G) == 0:
            return S
        if k == 0:
            return ()
        u = G[0][0]

        degs = getListOfDegs(G, V)
        maxDeg = 0
        for v in range(0, len(degs)):
            if degs[v] == 1:
                u = v
                break
            if degs[v] > maxDeg:
                u = v
                maxDeg = degs[v]

        neighbours = ()
        GMinusU = G.copy()
        for (x, y) in G:
            if x == u:
                neighbours = neighbours + (y,)
                GMinusU.remove((x, y))
            if y == u:
                neighbours = neighbours + (x,)
                GMinusU.remove((x, y))
        S0 = thirdSolution(GMinusU, k - 1, S + (u,))

        GMinusNeigh = G.copy()
        for n in neighbours:
            for (x, y) in GMinusNeigh.copy():
                if x == n or y == n:
                    GMinusNeigh.remove((x, y))
        S1 = thirdSolution(GMinusNeigh, k - len(neighbours), S + neighbours)

        if S0 != ():
            return S0
        else:
            return S1


    for k in range(1, V):
        S = ()
        C = thirdSolution(edges, k, S)
        if C != ():
            saveSolution(name + ".sol", C)
            print(name + " |solution| " + C.__str__())
            break
