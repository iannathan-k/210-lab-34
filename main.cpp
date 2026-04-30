#include <iostream>
#include <vector>
#include <queue> // Added for BFS and Dijkstra's/Prim's Priority Queue
#include <stack> // Added for DFS
#include <string>
#include <climits> // Added for INT_MAX used in Dijkstra's/Prim's

// COMSC-210 | Lab 34 | Ian Kusmiantoro & Google Gemini Pro & Github Copilot
/*
Dear Professor, I really appreciate this assignment, because I had a lot of fun messing around
with both the LLM and the algorithms, seeing what ideas it could come up with! I also appreciated
this chapter because I covered this in my discrete math class, so I know about Prim's, Kruskal's,
Dijkstra's, etc. LOL.
*/

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

    // Shortest Path - Dijkstra's Algorithm (Application Output)
    void shortestPath(int start) {
        cout << "Optimal Routing (Shortest Path) from Node " << start << " (" << nodeNames[start] << "):" << endl;
        cout << "Purpose: Finding minimum latency paths to all active servers" << endl;
        cout << "===========================================================" << endl;

        // Priority queue to store {distance, vertex} pairs. 
        // Using greater<Pair> to create a min-heap so the shortest distance is always on top.
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        
        // Initialize all distances to infinity (INT_MAX)
        vector<int> dist(SIZE, INT_MAX);

        // Insert source itself into priority queue and initialize its distance as 0
        pq.push(make_pair(0, start));
        dist[start] = 0;

        while (!pq.empty()) {
            // Get the minimum distance vertex
            int u = pq.top().second;
            pq.pop();

            // Iterate through all adjacent vertices of u
            for (auto &v : adjList[u]) {
                int vertex = v.first;
                int weight = v.second;

                // If there is a shorter path to v through u
                if (dist[u] != INT_MAX && dist[u] + weight < dist[vertex]) {
                    dist[vertex] = dist[u] + weight;
                    pq.push(make_pair(dist[vertex], vertex));
                }
            }
        }

        // Print shortest paths, formatted to match the sample output while keeping our app theme
        for (int i = 0; i < SIZE; i++) {
            // Only print if the node is actually reachable (skips our offline "deleted" nodes 3 and 4)
            if (dist[i] != INT_MAX) {
                cout << start << " -> " << i << " : " << dist[i] << " ms (" << nodeNames[i] << ")" << endl;
            }
        }
        cout << endl;
    }

    // Minimum Spanning Tree - Prim's Algorithm (Application Output)
    void MST(int start) {
        cout << "Minimum Spanning Tree edges:" << endl;
        cout << "Purpose: Finding the most efficient backbone to connect all servers" << endl;
        cout << "===========================================================" << endl;

        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        
        vector<int> key(SIZE, INT_MAX);   // Weights of the edges used to reach nodes
        vector<int> parent(SIZE, -1);     // Array to store the constructed MST
        vector<bool> inMST(SIZE, false);  // To keep track of vertices included in MST

        pq.push(make_pair(0, start));
        key[start] = 0;

        while (!pq.empty()) {
            // Get the minimum key vertex
            int u = pq.top().second;
            pq.pop();

            // If we've already included this node in the MST, skip it
            if (inMST[u]) continue;
            
            // Include vertex in MST
            inMST[u] = true;

            // Iterate through all adjacent vertices of u
            for (auto &v : adjList[u]) {
                int vertex = v.first;
                int weight = v.second;

                // If v is not yet in the MST and the weight of (u,v) is smaller than current key of v
                if (!inMST[vertex] && weight < key[vertex]) {
                    key[vertex] = weight;
                    pq.push(make_pair(key[vertex], vertex));
                    parent[vertex] = u;
                }
            }
        }

        // Print the MST, mirroring the sample structure but using our latency terminology
        for (int i = 0; i < SIZE; i++) {
            // Print only if a parent exists (skips the start node and offline nodes)
            if (parent[i] != -1) {
                cout << "Edge from " << i << " to " << parent[i] 
                     << " with latency: " << key[i] << " ms (" << nodeNames[i] << " <-> " << nodeNames[parent[i]] << ")" << endl;
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

    // The following code was generated by AI. (Actually Ian)
    int choice = -1;
    while (choice != 0) {
        cout << "Server Network Menu:" << endl;
        cout << "[1] Print Network Topology" << endl;
        cout << "[2] Analyze Broadcast Packet Propagation (BFS)" << endl;
        cout << "[3] Trace Simulated Cyberattack (DFS)" << endl;
        cout << "[4] Find Shortest Routing (Dijkstra's)" << endl;
        cout << "[5] Generate Minimum Spanning Tree (Prim's)" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter you choice: ";
        cin >> choice;

        if (choice == 1) {
            graph.printGraph();
        } else if (choice == 2) {
            graph.BFS(0);
        } else if (choice == 3) {
            graph.DFS(0);
        } else if (choice == 4) {
            graph.shortestPath(0);
        } else if (choice == 5) {
            graph.MST(0);
        }
        cout << endl;
    }

    return 0;
}