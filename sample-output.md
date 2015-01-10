# Find the single bridge in a graph 

This is an interesting problem posted by MapBox directions team. [ref](http://www.mapbox.com/blog/directions-hiring/)

My solution contains two steps: 

1. Randomly generate a single-bridge graph with a given number of vertex. 
2. Find the bridge with DFS traversal of the graph.

Here we go. 

## Step 1

Please input the number of vertex (at least 8) in the graph: 10000000
7114193 vertices are randomly selected to construct the 1st sub-graph
2885807 vertices are randomly selected to construct the 2nd sub-graph
Generating the single-bridge graph... 
done!
Randomly build a bridge between the two sub-graphs...  done!
The built bridge is (5966821, 9542817)
A graph with 10000000 vertices and 48672467 undirected edges has been constructed.
Time consumed: 108559 ms

## Step 2

Searching for the bridge...

Found it! The bridge is (5966821, 9542817)
Time consumed for finding the bridge: 15921 ms

Finished, thanks.

The codes is available at [github](https://github.com/tumluliu/mapbox-directions-hiring).
For any question or suggestion, please contact with Lu LIU (nudtlliu@gmail.com). 
