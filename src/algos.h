#include "graph.h"

#ifndef ALGOS_H_
#define ALGOS_H_

namespace algos{

    int printSolution(int dist[], int V)
    {
        cout<<"Vertex   Distance from Source"<<endl;
        for (int i = 0; i < V; i++)
            cout<<i<<"\t "<<dist[i]<<endl;
            //printf("%d \t\t %d\n", i, dist[i]);
    }
    // The main function that finds shortest distances from src to
    // all other vertices using Bellman-Ford algorithm.  The function
    // also detects negative weight cycle
    void BellmanFord(Graph g, int src, int * dist, int * pred)
    {
        int V = g.numVertex();
        int E = g.E;

        // Step 1: Initialize distances from src to all other vertices
        // as INFINITE and predecessor list as -INFINITE
        for (int i = 0; i < V; i++){
            dist[i] = INT_MAX;
            pred[i] = -INT_MAX;
        }
        dist[src] = 0;

        // Step 2: Relax all edges |V| - 1 times. A simple shortest
        // path from src to any other vertex can have at-most |V| - 1
        // edges
        for (int i = 1; i <= V-1; i++)
        {
            int change = 0;
            for (int u = 0; u < V; u++)
            {
                if (dist[u] == INT_MAX) continue;
                for (list<pair<int, int> >::const_iterator i = g.adj[u].begin(); i != g.adj[u].end(); i++)
                {
                    int v = i->first;
                    int weight = i->second;
                    if (dist[u] + weight < dist[v]){
                        dist[v] = dist[u] + weight;
                        pred[v] = u;
                        change = 1;
                    }
                }
            }
            if (!change) break;
        }

    }
}
#endif // ALGOS_H_
