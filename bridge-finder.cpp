// A C++ program to find bridges in a given undirected graph
#include <iostream>
#include <list>
#include <ctime>
#include <algorithm>
#define NIL -1
#define MAX_DEGREE 5
#define MIN_DEGREE 3

using namespace std;
 
// A class that represents an undirected graph
class Graph
{
    int V;    // No. of vertices
    list<int> *adj;    // A dynamic array of adjacency lists
    void bridgeUtil(int v, bool visited[], int disc[], int low[], int parent[]);
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // function to add an edge to graph
    void bridge();    // prints all bridges
};
 
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
    list<int> *subAdj1, *subAdj2;
    int V1, V2;
    srand(static_cast<unsigned int>(time(0)));
    V1 = rand() % V;
    V2 = V - V1;
    while (V1 < 4 || V2 < 4) {
        srand(static_cast<unsigned int>(time(0)));
        V1 = rand() % V;
        V2 = V - V1;
    }
    cout << V1 << " vertices are randomly selected to construct the 1st sub-graph" << endl;
    cout << V2 << " vertices are randomly selected to construct the 2nd sub-graph" << endl;
    // vertex id list: [0..V1-1], [V1, V-1]
    int degree = 0;
    for (int i = 0; i < V; i++) {
        // randomly determine the No. of neighbors for the current vertex
        srand(static_cast<unsigned int>(time(0)));
        degree = rand() % (MAX_DEGREE - MIN_DEGREE + 1) + MIN_DEGREE;
        cout << degree << " neighbors will be attached to vertex " << i << endl;
        //cout << adj[i].size() << " of them are already there " << endl;
        int n = degree - adj[i].size();
        //cout << "Aditional " << n << " ones are: " << endl;
        for (int j = 0; j < n; j++) {
            //cout << "loop index j: " << j << " ===> "; 
            // randomly determine these neighbors
            srand(static_cast<unsigned int>(time(0)));
            int randomNeighbor = i < V1 ? rand() % V1 : rand() % V2 + V1;
            bool found = (find(adj[i].begin(), adj[i].end(), randomNeighbor) != adj[i].end());
            while (randomNeighbor == i || found == true) {
                srand(static_cast<unsigned int>(time(0)));
                randomNeighbor = i < V1 ? rand() % V1 : rand() % V2 + V1;
                found = (find(adj[i].begin(), adj[i].end(), randomNeighbor) != adj[i].end());
            }
            //cout << randomNeighbor << endl;
            adj[i].push_back(randomNeighbor);
            adj[randomNeighbor].push_back(i);
        }
    }

    srand(static_cast<unsigned int>(time(0)));
    int bridgeVertex1 = rand() % V1;
    int bridgeVertex2 = rand() % V2 + V1;

    cout << "The finally constructed bridge is (" << bridgeVertex1 << ", " << bridgeVertex2 << ")" << endl;

    adj[bridgeVertex1].push_back(bridgeVertex2);
    adj[bridgeVertex2].push_back(bridgeVertex1);

}
 
void Graph::addEdge(int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);  // Note: the graph is undirected
}
 
// A recursive function that finds and prints bridges using DFS traversal
// u --> The vertex to be visited next
// visited[] --> keeps tract of visited vertices
// disc[] --> Stores discovery times of visited vertices
// parent[] --> Stores parent vertices in DFS tree
void Graph::bridgeUtil(int u, bool visited[], int disc[], 
                                       int low[], int parent[])
{
    // A static variable is used for simplicity, we can avoid use of static
    // variable by passing a pointer.
    static int time = 0;
 
    // Mark the current node as visited
    visited[u] = true;
 
    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
 
    // Go through all vertices aadjacent to this
    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i;  // v is current adjacent of u
 
        // If v is not visited yet, then recur for it
        if (!visited[v])
        {
            parent[v] = u;
            bridgeUtil(v, visited, disc, low, parent);
 
            // Check if the subtree rooted with v has a connection to
            // one of the ancestors of u
            low[u]  = min(low[u], low[v]);
 
            // If the lowest vertex reachable from subtree under v is 
            // below u in DFS tree, then u-v is a bridge
            if (low[v] > disc[u])
              cout << u <<" " << v << endl;
        }
 
        // Update low value of u for parent function calls.
        else if (v != parent[u])
            low[u]  = min(low[u], disc[v]);
    }
}
 
// DFS based function to find all bridges. It uses recursive function bridgeUtil()
void Graph::bridge()
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];
 
    // Initialize parent and visited arrays
    for (int i = 0; i < V; i++)
    {
        parent[i] = NIL;
        visited[i] = false;
    }
 
    // Call the recursive helper function to find Bridges
    // in DFS tree rooted with vertex 'i'
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            bridgeUtil(i, visited, disc, low, parent);
}
 
// Driver program to test above function
int main()
{
    // Create a graph with following attributes: 
    // 1. the number of vertex is given by user input;
    // 2. for each vertex, at least 3 neighbors should be linked;
    // 3. there must be ONLY one bridge in the graph.
    //
    
    int V;
    cout << "\n Generate a random graph with a single bridge..." << endl;
    cout << "Please input the number of vertex (at least 8) you want to have in the graph: ";
    cin >> V;
    while (V < 8) {
        cout << "Please input a number larger or equal to 8: ";
        cin >> V;
    }
    Graph g1(V);
    //g1.addEdge(1, 0);
    //g1.addEdge(0, 2);
    //g1.addEdge(2, 1);
    //g1.addEdge(0, 3);
    //g1.addEdge(3, 4);
    g1.bridge();
 
    return 0;
}
