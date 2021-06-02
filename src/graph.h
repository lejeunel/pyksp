#include <iostream>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>
#include <vector>
#include <climits>
#include <fstream>

using namespace std;

#ifndef GRAPH_H_
#define GRAPH_H_

// Graph class represents a directed graph
// using adjacency list representation
class Graph
{
public:
    int V = 0;    // No. of vertices
    int E = 0;    // No. of edges

    // Pointer to an array containing
    // adjacency lists
    list<pair<int, int> > *adj;

    Graph(int V);   // Constructor

    // function to add an edge to graph
    void addEdge(int v, int w, double weight);
    void printEdge();
    int numVertex();
    int numEdges(){ return E;}
};

int Graph::numVertex()
{
    return this->V;
}

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<pair<int, int> >[V];
}

void Graph::addEdge(int v, int w, double weight)
{

    // cout << "construct edge: (" << v << ", " << w << ", " << weight << ")" << endl;
    adj[v].push_back(make_pair(w, weight)); // Add w to v’s list.
    cout << "E: " << E << endl;
    ++E;

}

void Graph::printEdge()
{
    for (int k = 0; k < V; k++){
        for (list<pair<int, int> >::const_iterator i = adj[k].begin(); i != adj[k].end(); i++){
            cout << k << "," << i->first << "," << i->second;
            cout << endl;
        }
    }
}

#endif // GRAPH_H_
