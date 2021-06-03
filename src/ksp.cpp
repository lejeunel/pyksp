#include "ksp.h"

// std::vector<py::array_t<int>> Ksp::run(){
std::vector<py::array_t<int>> Ksp::run(){

    find_best_paths();
    retrieve_disjoint_paths();

    std::vector<py::array_t<int>> res;

    for(int i=0; i<n_paths; ++i){
        auto path_arr = py::array_t<int>(std::vector<ptrdiff_t>{paths[i]->n_vertices},
                                         paths[i]->vertices);
        paths[i]->print();
        res.push_back(path_arr);
        }

    return res;

}
