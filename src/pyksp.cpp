/*
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor(s): Laurent Lejeune (laurent.lejeune@artorg.unibe.ch).
 *
 */
#include <iostream>
#include <limits>
#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

#include "ksp.h"

class PyKsp{

    public:
        PyKsp(const int & n_vertices){

          ksp = new Ksp(n_vertices);
        };

        void init_from_adj(py::array_t<int> A, py::array_t<double> W);
        void run();
        void config(int a_l_max, bool a_min_cost, int a_source_id,
                          int a_sink_id, bool a_verbose){
          ksp->set_l_max(a_l_max);
          ksp->set_min_cost(a_min_cost);
          ksp->set_source_id(a_source_id);
          ksp->set_sink_id(a_sink_id);
          ksp->set_verbose(a_verbose);

        }

        std::string summary() const{
          stringstream ss;
          ss << ksp->numVertex() << " vertices, " << ksp->numEdges() << " edges";
          return  ss.str();
        }

    private:

        Ksp * ksp;

};


void PyKsp::init_from_adj(py::array_t<int> A, py::array_t<double> W){

  auto a = A.unchecked<2>(); // A must have ndim = 2; can be non-writeable
  auto w = W.unchecked<1>(); // W must have ndim = 1; can be non-writeable

  if(a.shape(0) != 2)
    throw py::value_error("Adjacency matrix must be of shape [2xN]");

  if(w.shape(0) != a.shape(1))
    throw py::value_error("Weights array must be of same length as adjancency matrix");

  for (py::ssize_t j = 0; j < a.shape(1); j++){
    // py::print("Adding edge (", a(0, j), ",", a(1, j), ")", "with weight: ", w(j));
     ksp->addEdge(a(0, j), a(1, j), w(j));
    // py::print("num edges: ", ksp->numEdges());
  }
}

void PyKsp::run(){
  auto path = ksp->run();
  // py::print("BellmanFord: ", res);


}

PYBIND11_MODULE(pyksp, m) {
    py::class_<PyKsp>(m, "PyKsp")
      .def(py::init<const int &>())
        .def("init_from_adj", &PyKsp::init_from_adj)
      .def("config", &PyKsp::config, "Set parameters",
      py::arg("l_max") = -1, py::arg("min_cost") = false,
            py::arg("source_id") = 0, py::arg("sink_id")=1, py::arg("verbose") = true)
      .def("run", &PyKsp::run, "Runs K-shortest paths")
      .def("__repr__",
              [](const PyKsp &a) {
                  return "<pyksp.PyKsp '" + a.summary() + "'>";
              });
}

