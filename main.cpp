#include <iostream>
#include <vector>
#include <queue> // Added for BFS
#include <stack> // Added for DFS

// COMSC-210 | Lab 34 | Ian Kusmiantoro & Google Gemini Pro & Github Copilot

using namespace std;

const int SIZE = 7;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

    // Depth-First Search
    void DFS(int start) {
        vector<bool> visited(SIZE, false); // ZyBooks uses an unordered_set, but we didn't learn that so I guess this will do - Ian
        stack<int> s; // Stack is used for DFS - Ian

        s.push(start);

        while (!s.empty()) {
            int curr = s.top();
            s.pop();

            // Check if visited here to handle duplicate pushes gracefully
            if (!visited[curr]) {
                cout << curr << " ";
                visited[curr] = true;
            }

            // Push neighbors to the stack
            for (Pair v : adjList[curr]) {
                if (!visited[v.first]) {
                    s.push(v.first);
                }
            }
        }
        cout << endl;
    }

    // Breadth-First Search
    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q; // Queue is used for BFS - Ian

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            cout << curr << " ";

            // Traverse all neighbors of the current vertex
            for (Pair v : adjList[curr]) {
                if (!visited[v.first]) {
                    visited[v.first] = true;
                    q.push(v.first);
                }
            }
        }
        cout << endl;
    }
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0, 1, 12},
        {0, 2, 8},
        {0, 3, 21},
        {2, 3, 6},
        {2, 6, 2},
        {5, 6, 6},
        {4, 5, 9},
        {2, 4, 4},
        {2, 5, 5}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();

    // DFS Driver thing
    cout << "DFS starting from vertex 0:" << endl;
    graph.DFS(0);

    // Bfs driver thingy
    cout << "BFS starting from vertex 0:" << endl;
    graph.BFS(0);

    return 0;
}