#include <vector>
#include <algorithm>
#include "globals.h"
#include "graph.h"
#include "algos.h"

#ifndef KSP_H_
#define KSP_H_

void print_path(std::vector<int> path){

    for(int i=0; i<path.size();++i){
        std::cout << path[i];
        if(i < path.size() - 1) std::cout << "->";
   }
    std::cout << std::endl;

}

std::vector<int> pred_to_path(int * pred, int src_id, int sink_id){

    std::vector<int> path;
    path.push_back(sink_id);
    int curr_id = sink_id;
    while(curr_id != src_id){
        path.push_back(pred[curr_id]);
        curr_id = pred[curr_id];
    }


   std::reverse(path.begin(), path.end());
   return path;

}

class Ksp : public Graph {

    public:
        Ksp(int V) : Graph(V){
        };

    void set_l_max(int l){
      l_max = l;
    }

    void set_min_cost(bool flag){
      min_cost = flag;
    }

    void set_verbose(bool flag){
      verbose = flag;
    }

    void set_source_id(int v){
      source_id = v;
    }

    void set_sink_id(int v){
      sink_id = v;
    }

    float run(){

        BellmanFord();
        Dijkstra();
    }

    private:

        //stopping conditions
        int l_max = -1; // maximum number of paths to find
        bool min_cost = true; // Find minimum cost or find all paths

        // indices of source and sink nodes
        int source_id = -1;
        int sink_id = -2;

        bool return_edges;
        bool verbose;

        double cost;
        double tol;
        double new_cost; // will store two consecutives costs for comparison

    std::vector<int> BellmanFord(){
        int dist[V];
        int pred[V];

        algos::BellmanFord(*this, source_id, dist, pred);
        auto path = pred_to_path(pred, source_id, sink_id);
        print_path(path);
        return path;

    }

    std::vector<int> Dijkstra(){
        int dist[V];
        int pred[V];
        bool sptSet[V];

        algos::dijkstra(*this, source_id, dist, pred, sptSet);
        auto path = pred_to_path(pred, source_id, sink_id);
        print_path(path);
        return path;


    }

};


#endif // TRACKER_H_
