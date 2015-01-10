/*
 * Copyright 2015 Lu LIU
 * =====================================================================================
 *
 *       Filename:  bridge-finder.cpp
 *
 *    Description:  A C++ program to find the single bridge in an undirected
 *                  graph. The problem is from MapBox directions team's hiring
 *                  page. Please refer to
 *                  http://www.mapbox.com/blog/directions-hiring/
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
#include <sys/time.h>
#include <iostream>
#include <list>
#include <ctime>
#include <cstdint>
#include <algorithm>
#define NIL -1
#define MAX_DEGREE 7
#define MIN_DEGREE 3

using std::cout;
using std::cin;
using std::endl;
using std::fixed;
using std::list;
using std::min;

typedef uint64_t uint64;
typedef unsigned int uint;

/*
 * =====================================================================================
 *        Class:  SingleBridgeGraph
 *  Description:  A class that represents a single-bridge undirected graph
 * =====================================================================================
 */
class SingleBridgeGraph {
  // number of vertices and edges
  int V, E;
  // A dynamic array of adjacency lists
  list<int> *adj;
  void bridgeProbe(int v, bool visited[], int disc[], int low[], int parent[]);
  uint64 getCurrentTimeMs();

public:
  explicit SingleBridgeGraph(int V);
  void addEdge(int v, int w);
  void findBridges();
};

/*
 *--------------------------------------------------------------------------------------
 *       Class:  SingleBridgeGraph
 *      Method:  SingleBridgeGraph :: SingleBridgeGraph
 * Description:  Create a graph with following attributes:
 *               1. the number of vertex is given by user input;
 *               2. for each vertex, random number (between MIN_DEGREE and
 *                  MAX_DEGREE) of neighbors will be attached;
 *               3. there must be ONLY one bridge in the graph.
 *--------------------------------------------------------------------------------------
 */
SingleBridgeGraph::SingleBridgeGraph(int V) {
  uint64 startTime = getCurrentTimeMs();
  this->V = V;
  this->E = 0;
  adj = new list<int>[V];
  // Two seperated sub-graphs will be constructed respectively.
  // Their number of vertices (V1 and V2) are randomly determined, but the sum
  // is V.
  int V1, V2;
  uint seed = static_cast<uint>(time(0));
  V1 = rand_r(&seed) % V;
  V2 = V - V1;
  // According to the "each vertex has at least 3 neighbors" request, there
  // should be at least 8 vertices in the graph (4 for each sub-graph).
  while (V1 < 4 || V2 < 4) {
    seed = static_cast<uint>(time(0));
    V1 = rand_r(&seed) % V;
    V2 = V - V1;
  }
  cout << endl << " " << V1
       << " vertices are randomly selected to construct the 1st sub-graph"
       << endl;
  cout << endl << " " << V2
       << " vertices are randomly selected to construct the 2nd sub-graph"
       << endl;
  // vertex id list for the two sub-graphs: [0..V1-1] and [V1, V-1]
  int degree = 0;
  cout << endl << " Generating the single-bridge graph... " << endl;
  double progress = 0.0;
  for (int i = 0; i < V; i++) {
    // randomly determine the number of neighbors for the current vertex
    progress = static_cast<double>(i + 1) / static_cast<double>(V) * 100.0;
    cout.precision(1);
    cout << fixed << progress << "%\r";
    cout.flush();
    seed = static_cast<uint>(time(0));
    degree = rand_r(&seed) % (MAX_DEGREE - MIN_DEGREE + 1) + MIN_DEGREE;
    int n = degree - adj[i].size();
    for (int j = 0; j < n; j++) {
      // randomly determine these neighbors
      // they should be unique, and not be the current vertex (otherwise there
      // will be a cycle)
      int randomNeighbor =
          i < V1 ? rand_r(&seed) % V1 : rand_r(&seed) % V2 + V1;
      bool found =
          (find(adj[i].begin(), adj[i].end(), randomNeighbor) != adj[i].end());
      while (randomNeighbor == i || found == true) {
        seed = static_cast<uint>(time(0));
        randomNeighbor = i < V1 ? rand_r(&seed) % V1 : rand_r(&seed) % V2 + V1;
        found = (find(adj[i].begin(), adj[i].end(), randomNeighbor) !=
                 adj[i].end());
      }
      addEdge(i, randomNeighbor);
    }
  }
  cout << " done!" << endl;
  cout << endl << " Randomly build a bridge between the two sub-graphs... ";
  seed = static_cast<uint>(time(0));
  int bridgeVertex1 = rand_r(&seed) % V1;
  int bridgeVertex2 = rand_r(&seed) % V2 + V1;
  addEdge(bridgeVertex1, bridgeVertex2);
  uint64 elapsedTime = getCurrentTimeMs() - startTime;
  cout << " done!" << endl;
  cout << endl << " The built bridge is (" << bridgeVertex1 << ", "
       << bridgeVertex2 << ")." << endl;
  cout << endl << " A graph with " << V << " vertices and " << this->E
       << " undirected edges has been constructed." << endl;
  cout << " Time consumed for graph construction: " << elapsedTime << " ms"
       << endl;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  SingleBridgeGraph
 *      Method:  SingleBridgeGraph :: addEdge
 * Description:  add forward and backward edges to the graph between u and v
 *--------------------------------------------------------------------------------------
 */
void SingleBridgeGraph::addEdge(int u, int v) {
  adj[u].push_back(v);
  adj[v].push_back(u);
  E++;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  SingleBridgeGraph
 *      Method:  SingleBridgeGraph :: getCurrentTimeMs
 * Description:  Get current time in milliseconds
 *--------------------------------------------------------------------------------------
 */
uint64 SingleBridgeGraph::getCurrentTimeMs() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  uint64 ret = tv.tv_usec;
  ret /= 1000;
  ret += (tv.tv_sec * 1000);
  return ret;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  SingleBridgeGraph
 *      Method:  SingleBridgeGraph :: bridgeProbe
 * Description:  A recursive function that finds and prints bridges using DFS
 *               traversal.
 *               u --> The vertex to be visited next
 *               visited[] --> keeps track of visited vertices
 *               disc[] --> Stores discovery times of visited vertices
 *               parent[] --> Stores parent vertices in DFS tree
 *--------------------------------------------------------------------------------------
 */
void SingleBridgeGraph::bridgeProbe(int u, bool visited[], int disc[],
                                    int low[], int parent[]) {
  static int time = 0;
  // Mark the current node as visited
  visited[u] = true;
  // Initialize discovery time and low value
  disc[u] = low[u] = ++time;
  // Go through all vertices adjacent to this
  list<int>::iterator i;
  for (i = adj[u].begin(); i != adj[u].end(); ++i) {
    int v = *i;
    // If v is not visited yet, then recur for it
    if (!visited[v]) {
      parent[v] = u;
      bridgeProbe(v, visited, disc, low, parent);
      // Check if the subtree rooted with v has a connection to
      // one of the ancestors of u
      low[u] = min(low[u], low[v]);
      // If the lowest vertex reachable from subtree under v is
      // below u in DFS tree, then u-v is a bridge
      if (low[v] > disc[u])
        cout << endl << " Found it! The bridge is (" << u << ", " << v << ")."
             << endl;
    } else if (v != parent[u]) {
      // Update low value of u for parent function calls.
      low[u] = min(low[u], disc[v]);
    }
  }
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  SingleBridgeGraph
 *      Method:  SingleBridgeGraph :: findBridges
 * Description:  DFS based function to find all bridges. It uses recursive
 *               function bridgeProbe(). Please note that the method is not
 *               invented by me. The findBridges() and bridgeProbe() functions
 *               are modified from that on
 *               [geeksforgeeks.org](http://www.geeksforgeeks.org/bridge-in-a-graph/)
 *--------------------------------------------------------------------------------------
 */
void SingleBridgeGraph::findBridges() {
  // Mark all the vertices as not visited
  bool *visited = new bool[V];
  int *disc = new int[V];
  int *low = new int[V];
  int *parent = new int[V];

  uint64 startTime = getCurrentTimeMs();
  // Initialize parent and visited arrays
  for (int i = 0; i < V; i++) {
    parent[i] = NIL;
    visited[i] = false;
  }

  // Call the recursive helper function to find Bridges
  // in DFS tree rooted with vertex 'i'
  for (int i = 0; i < V; i++)
    if (visited[i] == false)
      bridgeProbe(i, visited, disc, low, parent);
  uint64 elapsedTime = getCurrentTimeMs() - startTime;
  cout << " Time consumed for finding the bridge: " << elapsedTime << " ms"
       << endl;
}

int main() {
  int V;
  cout << endl << " # Find the single bridge in a graph " << endl << endl;
  cout << " This is an interesting problem posted by MapBox directions team. "
          "[ref](http://www.mapbox.com/blog/directions-hiring/)" << endl
       << endl;
  cout << " My solution contains two steps: " << endl << endl;
  cout << " 1. Randomly generate a single-bridge graph with a given number of "
          "vertex. " << endl;
  cout << " 2. Find the bridge with DFS traversal of the graph. " << endl
       << endl;
  cout << " Here we go. " << endl;
  cout << endl << " ## Step 1" << endl << endl;
  cout << " Please input the number of vertex (at least 8) in the graph: ";
  cin >> V;
  while (V < 8) {
    cout << " Please input a number larger or equal to 8: ";
    cin >> V;
  }

  SingleBridgeGraph g(V);
  cout << endl << " ## Step 2" << endl << endl;
  cout << " Searching for the bridge..." << endl;
  g.findBridges();
  cout << endl << " Finished, thanks." << endl << endl;
  cout << " The codes are available at "
          "[github](https://github.com/tumluliu/mapbox-directions-hiring)."
       << endl;
  cout << " For any question or suggestion, please contact with Lu LIU "
          "(nudtlliu@gmail.com). " << endl;

  return 0;
}
