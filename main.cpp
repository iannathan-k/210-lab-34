#include <iostream>
#include <vector>
#include <queue> // Added for BFS
#include <stack> // Added for DFS
#include <string>

// COMSC-210 | Lab 34 | Ian Kusmiantoro & Google Gemini Pro & Github Copilot

using namespace std;

// Increased SIZE to 13 to handle the new nodes (up to node 12)
const int SIZE = 13;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;
    vector<string> nodeNames; // Added to store our real-world application names

    // Graph Constructor
    Graph(vector<Edge> const &edges, vector<string> const &names) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);
        nodeNames = names;

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

    // Print the graph's adjacency list (Application Output)
    void printGraph() {
        cout << "Server Network Topology:" << endl;
        cout << "================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            // Skip printing nodes that have no edges (our "deleted" nodes)
            if (!adjList[i].empty()) {
                cout << "Node " << i << " (" << nodeNames[i] << ") connects to:" << endl;
                for (Pair v : adjList[i]) {
                    cout << "  -> Node " << v.first << " (" << nodeNames[v.first] 
                         << ") - Latency: " << v.second << " ms" << endl;
                }
            }
        }
        cout << endl;
    }

    // Depth-First Search (Application Output)
    void DFS(int start) {
        cout << "Network Trace (DFS) from Node " << start << " (" << nodeNames[start] << "):" << endl;
        cout << "Purpose: Tracing simulated cyber attack path through the subnets" << endl;
        cout << "=======================================" << endl;

        vector<bool> visited(SIZE, false); // ZyBooks uses an unordered_set, but we didn't learn that so I guess this will do - Ian
        stack<int> s; // Stack is used for DFS - Ian

        s.push(start);

        while (!s.empty()) {
            int curr = s.top();
            s.pop();

            // Check if visited here to handle duplicate pushes gracefully
            if (!visited[curr]) {
                cout << "Inspecting Node " << curr << " (" << nodeNames[curr] << ")" << endl;
                visited[curr] = true;

                // Push neighbors to the stack
                for (Pair v : adjList[curr]) {
                    if (!visited[v.first]) {
                        cout << "  -> Potential spread to Node " << v.first 
                             << " (" << nodeNames[v.first] << ") - Latency: " << v.second << " ms" << endl;
                        s.push(v.first);
                    }
                }
            }
        }
        cout << endl;
    }

    // Breadth-First Search (Application Output)
    void BFS(int start) {
        cout << "Layer-by-Layer Network Inspection (BFS) from Node " << start << " (" << nodeNames[start] << "):" << endl;
        cout << "Purpose: Analyzing broadcast packet propagation by hop count" << endl;
        cout << "=================================================" << endl;

        vector<bool> visited(SIZE, false);
        queue<int> q; // Queue is used for BFS - Ian

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            cout << "Checking Node " << curr << " (" << nodeNames[curr] << ")" << endl;

            // Traverse all neighbors of the current vertex
            for (Pair v : adjList[curr]) {
                if (!visited[v.first]) {
                    cout << "  -> Next reachable node: Node " << v.first 
                         << " (" << nodeNames[v.first] << ") - Latency: " << v.second << " ms" << endl;
                    visited[v.first] = true;
                    q.push(v.first);
                }
            }
        }
        cout << endl;
    }
};

int main() {
    // Application Mappings: Server/Hardware names assigned to nodes 0 through 12.
    // Notice nodes 3 and 4 are labeled "OFFLINE" to reflect our deleted nodes.
    vector<string> serverNames = {
        "Main Gateway", "Load Balancer A", "Load Balancer B", 
        "OFFLINE", "OFFLINE", "Web Server Alpha", "Web Server Beta", 
        "Database Server Primary", "Database Server Replica", 
        "Cache Server", "Backup Storage", "Analytics Engine", "Log Server"
    };

    // Creates a vector of graph edges/weights (Latency in ms)
    vector<Edge> edges = {
        {0, 1, 15},
        {0, 2, 10},
        {1, 5, 7},
        {1, 7, 14},
        {2, 6, 22},
        {2, 8, 5},
        {5, 9, 3},
        {7, 10, 9},
        {6, 11, 12},
        {8, 12, 1}
    };

    // Creates graph passing in the edges and our new server names
    Graph graph(edges, serverNames);

    // Prints adjacency list representation of graph
    graph.printGraph();

    // DFS Driver thing
    graph.DFS(0);

    // Bfs driver thingy
    graph.BFS(0);

    return 0;
}