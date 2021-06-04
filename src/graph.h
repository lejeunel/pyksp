 #include <iostream>
#include <float.h>
#include <cmath>
#include "misc.h"
#include "path.h"

using namespace std;

#ifndef GRAPH_H_
#define GRAPH_H_

class Vertex;
class Edge;

// Graph class represents a directed graph
// using adjacency list representation
class Graph
{
    public:

        Graph(int n_vertices, int n_edges,
            int * vertex_from, int * vertex_to,
              scalar_t * weights,
            int source, int sink);   // Constructor

        ~Graph();


        int get_n_vertices() {return _n_vertices;}
        int get_n_edges(){ return _n_edges;}
        Edge * get_edges(){return _edges;}
        Vertex * get_vertices(){return _vertices;}

        void find_best_paths();
        void update_positivized_lengths();

        void force_positivized_lengths();
        Path * retrieve_path();
        void retrieve_disjoint_paths();
        int retrieve_one_path(Edge *e, Path *path, int *used_edges);

        // These variables are filled when retrieve_disjoint_paths is called
        int n_paths;
        Path **paths;

        // shortest paths algorithms
        void dp_compute_distances();
        void Dijkstra();
        // Fills _dp_order
        void compute_dp_ordering();

        void set_min_cost(bool);
        void set_verbose(bool);
        void set_l_max(int);
    private:
        int _n_vertices = 0;
        int _n_edges = 0;

        Vertex * _source;
        Vertex * _sink;

        Edge * _edges;
        Vertex * _vertices;

        // For Dijkstra
        Vertex ** _heap;

        // Updating the distances from the source in that order will work in
        // the original graph (which has to be a DAG)
        Vertex **_dp_order;

        // parameters
        bool min_cost = true;
        bool verbose = false;
        float tolerance = 0;
        float l_max = -1;

};



#endif // GRAPH_H_
