#include <vector>
#include <algorithm>
#include "graph.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

#ifndef KSP_H_
#define KSP_H_


class Ksp : public Graph {

    public:

        Ksp(int n_vertices, int n_edges, int * vertex_from,
            int* vertex_to,
            scalar_t * weights,
            int source, int sink) : Graph(n_vertices,
                                          n_edges,
                                          vertex_from,
                                          vertex_to,
                                          weights,
                                          source,
                                          sink){};

        std::vector<py::array_t<int>> run();



};


#endif // TRACKER_H_
