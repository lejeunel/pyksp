#include "graph.h"

#ifndef ALGOS_H_
#define ALGOS_H_

namespace algos{

    int minDistance(int dist[], bool sptSet[], int V)
    {
        // Initialize min value
        int min = INT_MAX, min_index;

        for (int v = 0; v < V; v++)
        {
            if (sptSet[v] == false && dist[v] <= min)
                min = dist[v], min_index = v;
        }
        return min_index;
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

    float dijkstra(Graph g, int src, int * dist, int * pred, bool * sptSet)
    {
        int V = g.numVertex();

        // Initialize all distances as INFINITE and stpSet[] as false
        for (int i = 0; i < V; i++)
            dist[i] = INT_MAX, sptSet[i] = false, pred[i] = -INT_MAX;

        // Distance of source vertex from itself is always 0
        dist[src] = 0;

        // Find shortest path for all vertices
        for (int count = 0; count < V-1; count++)
        {
            // Pick the minimum distance vertex from the set of vertices not
            // yet processed. u is always equal to src in first iteration.
            int u = minDistance(dist, sptSet, V);

            // Mark the picked vertex as processed
            sptSet[u] = true;

            if (dist[u] == INT_MAX) break;
            // Update dist value of the adjacent vertices of the picked vertex.
            for (list<pair<int, int> >::const_iterator i = g.adj[u].begin(); i != g.adj[u].end(); i++)
            {
                int v = i->first;
                // Update dist[v] only if is not in sptSet, there is an edge from
                // u to v, and total weight of path from src to  v through u is
                // smaller than current value of dist[v]
                if (!sptSet[v] && dist[u]+ i->second < dist[v]){
                    dist[v] = dist[u] + i->second;
                    pred[v] = u;
                }

            }
        }
    }
}
#endif // ALGOS_H_
