/*

8 6 2 12
Algocja1
|---+.|.
++--++++
.|...|.|
.++--+-+
.|++.|..
.+-+.|.+

 */
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

struct Edge {
    int src, dest;
};

class Graph{
private:
    vector<vector<int>> adjList;
    int verticesNumber;
public:
    Graph(vector<Edge> const &edges, int N);
    void print();
    int getMinimumDegreeVertex();
    int getVerticesNumber();
    vector<int> getNeighbours(int v);
    void deleteVertex(int v);
};

Graph::Graph(vector<Edge> const &edges, int N){
    adjList.resize(N);
    for (auto &edge: edges)
    {
        adjList[edge.src].push_back(edge.dest);
    }
    verticesNumber = N;
}

void Graph::print() {
    for (int i = 0; i < verticesNumber; i++){
        cout << i << " --> ";

        for (int v : adjList[i])
            cout << v << " ";
        cout << endl;
    }
}

int Graph::getMinimumDegreeVertex() {
    int minDegree = 1000000;
    int vertex = -1;
    for (int i = 0; i < verticesNumber; i++){
        if(adjList[i].size() < minDegree && !adjList[i].empty()){
            minDegree = adjList[i].size();
            vertex = i;
        }
    }
    return vertex;
}

int Graph::getVerticesNumber() {
    return verticesNumber;
}

vector<int> Graph::getNeighbours(int v) {
    return adjList[v];
}

vector<int> solution = {};

void Graph::deleteVertex(int v) {
    for(auto u: adjList[v]){
        int index = 0;
        for(auto s: adjList[u]){
            if(s != v) index++;
            else break;
        }
        adjList[u].erase(adjList[u].begin()+index);
    }
    adjList[v].clear();
}


int W, H, L, K;

void printCoordinatesOf(int x){
    cout << x % W << " " << x / W << endl;
}

bool contains(const vector<Edge>& edges, Edge e){
    for(auto x: edges){
        if(x.dest == e.dest && x.src == e.src) return true;
    }
    return false;
}

int main() {
    cin >> W >> H >> L >> K;
    string name;
    cin >> name;
    char ** input = new char*[H];
    for(int i = 0; i < H; i++){
        input[i] = new char[W];
        string line;
        cin >> line;
        for(int j = 0; j < W; j++){
            input[i][j] = line[j];
        }
    }
    vector<Edge> edges = {};

    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            if(input[i][j] == '.') continue;
            bool zeroDegree = true;
            switch(input[i][j]){
                case '+':
                    if(i > 0 && (input[i-1][j] == '+' || input[i-1][j] == '|')){
                        edges.push_back({i*W+j,(i-1)*W+j}); zeroDegree = false;
                    }
                    if(i < H-1 && (input[i+1][j] == '+' || input[i+1][j] == '|')){
                        edges.push_back({i*W+j,(i+1)*W+j}); zeroDegree = false;
                    }
                    if(j > 0 && (input[i][j-1] == '+' || input[i][j-1] == '-')){
                        edges.push_back({i*W+j-1,i*W+j}); zeroDegree = false;
                    }
                    if(j < W-1 && (input[i][j+1] == '+' || input[i][j+1] == '-')){
                        edges.push_back({i*W+j+1,i*W+j}); zeroDegree = false;
                    }
                    break;
                case '-':
                    if(j > 0 && (input[i][j-1] == '+' || input[i][j-1] == '-')){
                        edges.push_back({i*W+j-1,i*W+j}); zeroDegree = false;
                    }
                    if(j < W-1 && (input[i][j+1] == '+' || input[i][j+1] == '-')){
                        edges.push_back({i*W+j+1,i*W+j}); zeroDegree = false;
                    }
                    break;
                case '|':
                    if(i > 0 && (input[i-1][j] == '+' || input[i-1][j] == '|')){
                        edges.push_back({i*W+j,(i-1)*W+j}); zeroDegree = false;
                    }
                    if(i < H-1 && (input[i+1][j] == '+' || input[i+1][j] == '|')){
                        edges.push_back({i*W+j,(i+1)*W+j}); zeroDegree = false;
                    }
                    break;
                default:
                    break;
            }
            if(zeroDegree) solution.push_back(i*W+j);
        }
    }
    for(int i = 0; i < H; i++) delete input[i];
    delete []input;
    int N = W*H;

    Graph graph(edges, N);

    vector<Edge> newEdges = edges;
    for(int v = 0; v < W*H; v++) {
        queue<int> toVisit;
        int *visited = new int[graph.getVerticesNumber()];
        int *distance = new int[graph.getVerticesNumber()];
        for (int i = 0; i < graph.getVerticesNumber(); i++) {
            visited[i] = 0;
        }
        visited[v] = 1;
        distance[v] = 0;
        toVisit.push(v);
        while (!toVisit.empty()) {
            int vertex = toVisit.front();
            toVisit.pop();
            if(v!=vertex && !contains(newEdges, {v, vertex})) newEdges.push_back({v, vertex});
            if (distance[vertex] == L) continue;
            for (auto neigh: graph.getNeighbours(vertex)) {
                if (visited[neigh] == 0) {
                    visited[neigh] = 1;
                    distance[neigh] = distance[vertex] + 1;
                    toVisit.push(neigh);
                }
            }
            visited[vertex] = 2;
        }
        delete[] visited;
        delete[] distance;
    }

    Graph graphL = Graph(newEdges, N);

    while(solution.size() < K){
        int v = graphL.getMinimumDegreeVertex();
        if(v == -1) break;
        solution.push_back(v);
        auto neighbours = graphL.getNeighbours(v);
        for(auto neigh: neighbours){
            graphL.deleteVertex(neigh);
        }
        graphL.deleteVertex(v);
    }

    for(auto x: solution) printCoordinatesOf(x);

    return 0;
}
