#include "graph.h"
#include <climits>

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

}
#endif // ALGOS_H_
