/*
 * =====================================================================================
 *
 *       Filename:  bridge-finder.cpp
 *
 *    Description:  A C++ program to find the single bridge in an undirected graph. 
 *                  The problem is from MapBox directions team's hiring page. Please 
 *                  refer to http://www.mapbox.com/blog/directions-hiring/
 *
 *        Version:  1.0
 *        Created:  2015/01/10 16时35分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *             OS:  Mac OS X 10.10.1
 *
 *         Author:  Lu LIU (lliu), nudtlliu@gmail.com
 *   Organization:  higis@dbrg
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <iostream>
#include <list>
#include <sys/time.h>
#include <ctime>
#include <algorithm>
#define NIL -1
#define MAX_DEGREE 7
#define MIN_DEGREE 3

using namespace std;
typedef unsigned long long uint64;

// A class that represents an undirected graph
class Graph
{
    int V;    // No. of vertices
    int E;    // No. of edges
    list<int> *adj;    // A dynamic array of adjacency lists
    void bridgeProbe(int v, bool visited[], int disc[], int low[], int parent[]);
    uint64 getCurrentTimeMs();
    public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // function to add an edge to graph
    void bridge();    // prints all bridges
};

Graph::Graph(int V)
{
    // Create a graph with following attributes: 
    // 1. the number of vertex is given by user input;
    // 2. for each vertex, [MIN_DEGREE, MAX_DEGREE] neighbors will be attached;
    // 3. there must be ONLY one bridge in the graph.
    uint64 startTime = getCurrentTimeMs();
    this->V = V;
    this->E = 0;
    adj = new list<int>[V];
    // Two seperated sub-graphs will be constructed respectively. 
    // Their number of vertices (V1 and V2) are randomly determined, but the sum is V. 
    int V1, V2;
    srand(static_cast<unsigned int>(time(0)));
    V1 = rand() % V;
    V2 = V - V1;
    // According to the "each vertex has at least 3 neighbors" request, there should be 
    // at least 8 vertices in the graph (4 for each sub-graph).
    while (V1 < 4 || V2 < 4) {
        srand(static_cast<unsigned int>(time(0)));
        V1 = rand() % V;
        V2 = V - V1;
    }
    cout << " " << V1 << " vertices are randomly selected to construct the 1st sub-graph" << endl;
    cout << " " << V2 << " vertices are randomly selected to construct the 2nd sub-graph" << endl;
    // vertex id list for the two sub-graphs: [0..V1-1] and [V1, V-1]
    int degree = 0;
    cout << " Generating the single-bridge graph... " << endl;
    double progress = 0.0;
    for (int i = 0; i < V; i++) {
        // randomly determine the number of neighbors for the current vertex
        progress = (double)(i + 1) / (double)V * 100.0;
        cout.precision(1);
        cout << fixed << progress << "%\r";
        cout.flush();
        srand(static_cast<unsigned int>(time(0)));
        degree = rand() % (MAX_DEGREE - MIN_DEGREE + 1) + MIN_DEGREE;
        int n = degree - adj[i].size();
        for (int j = 0; j < n; j++) {
            // randomly determine these neighbors
            // they should be unique, and not be the current vertex (otherwise there will be a cycle)
            int randomNeighbor = i < V1 ? rand() % V1 : rand() % V2 + V1;
            bool found = (find(adj[i].begin(), adj[i].end(), randomNeighbor) != adj[i].end());
            while (randomNeighbor == i || found == true) {
                srand(static_cast<unsigned int>(time(0)));
                randomNeighbor = i < V1 ? rand() % V1 : rand() % V2 + V1;
                found = (find(adj[i].begin(), adj[i].end(), randomNeighbor) != adj[i].end());
            }
            addEdge(i, randomNeighbor);
        }
    }
    cout << " done!" << endl;
    cout << " Randomly build a bridge between the two sub-graphs... ";
    srand(static_cast<unsigned int>(time(0)));
    int bridgeVertex1 = rand() % V1;
    int bridgeVertex2 = rand() % V2 + V1;
    addEdge(bridgeVertex1, bridgeVertex2);
    uint64 elapsedTime = getCurrentTimeMs() - startTime;
    cout << " done!" << endl;
    cout << " The built bridge is (" << bridgeVertex1 << ", " << bridgeVertex2 << ")" << endl;
    cout << " A graph with " << V << " vertices and " << this->E << " undirected edges has been constructed." << endl;
    cout << " Time consumed: " << elapsedTime << " ms" << endl;
}

void Graph::addEdge(int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);  // Note: the graph is undirected
    E++;
}

uint64 Graph::getCurrentTimeMs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64 ret = tv.tv_usec;
    ret /= 1000;
    ret += (tv.tv_sec * 1000);
    return ret;
}

// A recursive function that finds and prints bridges using DFS traversal
// u --> The vertex to be visited next
// visited[] --> keeps tract of visited vertices
// disc[] --> Stores discovery times of visited vertices
// parent[] --> Stores parent vertices in DFS tree
void Graph::bridgeProbe(int u, bool visited[], int disc[], 
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
            bridgeProbe(v, visited, disc, low, parent);

            // Check if the subtree rooted with v has a connection to
            // one of the ancestors of u
            low[u]  = min(low[u], low[v]);

            // If the lowest vertex reachable from subtree under v is 
            // below u in DFS tree, then u-v is a bridge
            if (low[v] > disc[u])
                cout << endl << " Found it! The bridge is (" << u << ", " << v << ")" << endl;
        }

        // Update low value of u for parent function calls.
        else if (v != parent[u])
            low[u]  = min(low[u], disc[v]);
    }
}

// DFS based function to find all bridges. It uses recursive function bridgeProbe()
void Graph::bridge()
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];

    uint64 startTime = getCurrentTimeMs();
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
            bridgeProbe(i, visited, disc, low, parent);
    uint64 elapsedTime = getCurrentTimeMs() - startTime;
    cout << " Time consumed for finding the bridge: " << elapsedTime << " ms" << endl;
}

// Driver program to test above function
int main()
{
    //

    int V;
    cout << endl << " # Find the single bridge in a graph " << endl << endl;
    cout << " This is an interesting problem posted by MapBox directions team. [ref](http://www.mapbox.com/blog/directions-hiring/)" << endl << endl;
    cout << " My solution contains two steps: " << endl << endl;
    cout << " 1. Randomly generate a single-bridge graph with a given number of vertex. " << endl;
    cout << " 2. Find the bridge with Tarjon's bridge-finding algorithm. " << endl << endl;
    cout << " Here we go. " << endl;
    cout << endl << " ## Step 1" << endl << endl;
    cout << " Please input the number of vertex (at least 8) in the graph: ";
    cin >> V;
    while (V < 8) {
        cout << "Please input a number larger or equal to 8: ";
        cin >> V;
    }
    Graph g1(V);
    cout << endl << " ## Step 2" << endl << endl;
    cout << " Searching for the bridge..." << endl;
    g1.bridge();
    cout << endl << " Finished, thanks." << endl << endl;
    cout << " The codes are available at [github](https://github.com/tumluliu/mapbox-directions-hiring)." << endl;
    cout << " For any question or suggestion, please contact with Lu LIU (nudtlliu@gmail.com). " << endl;

    return 0;
}
