import dimacs
import pycosat

G = dimacs.loadGraph("Graphs/2-FullIns_4.col")
V = len(G)
E = dimacs.edgeList(G)
colors = 0
solution = []


def index(i, j):
    return (i + j) * int((i + j + 1) / 2) + i + 1


# SAT reduction

for k in range(0, V + 1):
    cnf = []
    for vertex in range(0, V):
        clause = []
        for color in range(0, k):
            clause.append(index(vertex, color))
        cnf.append(clause)
        for color1 in range(0, k):
            for color2 in range(color1+1, k):
                clause = []
                clause.append(-index(vertex, color1))
                clause.append(-index(vertex, color2))
                cnf.append(clause)
        for edge in E:
            if edge[0] == vertex:
                for color in range(0, k):
                    clause = []
                    clause.append(-index(vertex, color))
                    clause.append(-index(edge[1], color))
                    cnf.append(clause)
    solution = pycosat.solve(cnf)
    if solution != 'UNSAT':
        print("Solution: "+solution.__str__())
        print("Number of colours: "+k.__str__())
        colors = k
        break

# get colors of vertices

color_list = []
for vertex in range(0, V):
    v_color = -1
    for color in range(0, colors):
        i = index(vertex, color)
        for variable in solution:
            if abs(variable) == i:
                if variable > 0:
                    v_color = color
                break
        if v_color >= 0:
            color_list.append((vertex, v_color))
            break
vertex_colors = dict(color_list)

# check solution

isCorrect = True
for edge in E:
    if vertex_colors[edge[0]] == vertex_colors[edge[1]]:
        print("Incorrect!")
        isCorrect = False
        break

if isCorrect:
    print("Correct!")
    print(vertex_colors)
