# Find the single bridge in a graph 

This is an interesting problem posted by MapBox directions team. [ref](http://www.mapbox.com/blog/directions-hiring/)

My solution contains two steps: 

1. Randomly generate a single-bridge graph with a given number of vertex. 
2. Find the bridge with DFS traversal of the graph. 

Here we go. 

## Step 1

Please input the number of vertex (at least 8) in the graph: 1000000

851617 vertices are randomly selected to construct the 1st sub-graph

148383 vertices are randomly selected to construct the 2nd sub-graph

Generating the single-bridge graph... 
done!

Randomly build a bridge between the two sub-graphs...  done!

The built bridge is (520311, 923965).

A graph with 1000000 vertices and 4939490 undirected edges has been constructed.
Time consumed for graph construction: 9084 ms

## Step 2

Searching for the bridge...

Found it! The bridge is (520311, 923965).
Time consumed for finding the bridge: 458 ms

Finished, thanks.

The codes are available at [github](https://github.com/tumluliu/mapbox-directions-hiring).
For any question or suggestion, please contact with Lu LIU (nudtlliu@gmail.com).
