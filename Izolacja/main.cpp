#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#define WHITE 0
#define GRAY 1
#define BLACK 2

struct Edge {
    int source;
    int destination;
};

class Graph{
    private:
        vector<vector<int>> adjList;
        int verticesNumber;
        vector<int> nonZeroVertices; //because deleted/not existing vertices and vertices with 0 degree have empty adjList
                                     //I have to store vertices with 0 degree
    public:
        Graph(vector<Edge> const &edges, int N);
        void print(); //for debug
        int getMinimumDegreeVertex();
        vector<int> getNeighbours(int v);
        void deleteVertex(int v);
        vector<int> getNonZeroVertices();
};

Graph::Graph(vector<Edge> const &edges, int N){
    adjList.resize(N);
    for (auto &edge: edges)
        adjList[edge.source].push_back(edge.destination);
    verticesNumber = N;
    nonZeroVertices = {};
    for(int v = 0; v < verticesNumber; v++)
        if(!adjList[v].empty()) nonZeroVertices.push_back(v);
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
    int resultVertex = -1;
    for (int v = 0; v < verticesNumber; v++){
        if(adjList[v].size() < minDegree && !adjList[v].empty()){ //degree from [1;minDegree)
            minDegree = adjList[v].size();
            resultVertex = v;
        }
    }
    return resultVertex;
}

vector<int> Graph::getNeighbours(int v) {
    return adjList[v];
}

vector<int> Graph::getNonZeroVertices() {
    return nonZeroVertices;
}


//global variables
int W, H, L, K;
vector<int> solution = {};

void Graph::deleteVertex(int v) {
    //delete all edges from v's neighbours
    for(auto u: adjList[v]){
        int index = 0;
        for(auto s: adjList[u]){
            if(s != v) index++;
            else break;
        }
        adjList[u].erase(adjList[u].begin()+index);
    }
    //delete v
    adjList[v].clear();
    //remove v from nonZeroVertices because it is deleted
    int index = 0;
    for(auto s: nonZeroVertices){
        if(s!=v) index++;
        else break;
    }
    nonZeroVertices.erase(nonZeroVertices.begin()+index);
}

void printCoordinatesOf(int x){
    cout << x % W << " " << x / W << endl;
}

bool contains(const vector<Edge>& edges, Edge e){
    for(auto x: edges){
        if(x.destination == e.destination && x.source == e.source) return true;
    }
    return false;
}

int main() {
    //read input
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

    //transform input into graph
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
            //if current vertex has 0 degree -> add to solution
            if(zeroDegree && solution.size() < K) {
                solution.push_back(i*W+j);
            }
        }
    }
    for(int i = 0; i < H; i++) delete input[i];
    delete []input;

    int N = W*H; //vertices number

    Graph graph(edges, N);

    //create graph^L using BFS
    vector<Edge> newEdges = edges;
    for(int v = 0; v < N; v++) {
        //BFS
        queue<int> toVisit;
        int *colour = new int[N];
        int *distance = new int[N];
        for (int i = 0; i < N; i++)
            colour[i] = WHITE;
        colour[v] = GRAY;
        distance[v] = 0;
        toVisit.push(v);
        while (!toVisit.empty()) {
            int currentV = toVisit.front();
            toVisit.pop();
            //add new edge
            if(v != currentV && !contains(newEdges, {v, currentV})) newEdges.push_back({v, currentV});
            if (distance[currentV] == L) continue; //because creating graph^L
            for (auto neigh: graph.getNeighbours(currentV)) {
                if (colour[neigh] == 0) {
                    colour[neigh] = GRAY;
                    distance[neigh] = distance[currentV] + 1;
                    toVisit.push(neigh);
                }
            }
            colour[currentV] = BLACK;
        }
        delete[] colour;
        delete[] distance;
    }

    Graph graphL = Graph(newEdges, N);

    //solving Independent Set Problem - removing vertex with minimum degree from graph^L
    while(solution.size() < K){
        int v = graphL.getMinimumDegreeVertex();
        if(v == -1) break; //when there is no vertices with non zero degree getMinimumDegreeVertex returns -1
        solution.push_back(v);
        auto neighbours = graphL.getNeighbours(v);
        for(auto neigh: neighbours){
            graphL.deleteVertex(neigh);
        }
        graphL.deleteVertex(v);
    }

    //if size of solution is still too small,
    //we can process nonZeroVertices attribute of graphL and add vertices with 0 degree to solution
    if(solution.size() < K) {
        for(auto v: graphL.getNonZeroVertices()){
            if(graphL.getNeighbours(v).empty()) {
                solution.push_back(v);
                if (solution.size() == K) break;
            }
        }
    }

    for(auto x: solution) printCoordinatesOf(x);

    return 0;
}
