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
#include <pybind11/stl.h>

namespace py = pybind11;

#include "ksp.h"

class PyKsp{

    public:
    PyKsp(py::array_t<int> vertices_from,py::array_t<int> vertices_to,
          py::array_t<scalar_t> W, int n_vertices, int source, int sink);
        ~PyKsp();

        std::vector<py::array_t<int>> run();
        void config(float tolerance, bool a_min_cost, bool a_verbose, int a_l_max){
          ksp->set_tolerance(tolerance);
          ksp->set_l_max(a_l_max);
          ksp->set_min_cost(a_min_cost);
          ksp->set_verbose(a_verbose);

        }

        std::string summary() const{
          stringstream ss;
          ss << ksp->get_n_vertices() << " vertices, " << ksp->get_n_edges() << " edges";
          return  ss.str();
        }

    private:

        Ksp * ksp;

};


PyKsp::PyKsp(py::array_t<int> vertices_from, py::array_t<int> vertices_to,
             py::array_t<scalar_t> weights,
             int n_vertices,
                        int source, int sink){

  // must have ndim = 1; can be non-writeable
  auto _vertices_from = vertices_from.unchecked<1>();
  auto _vertices_to = vertices_to.unchecked<1>();
  auto _weights = weights.unchecked<1>();

  if((_weights.shape(0) != _vertices_from.shape(0)) | (_weights.shape(0) != _vertices_to.shape(0)))
    throw py::value_error("Weights array must be of same length as adjacency matrix");

  int n_edges = _vertices_from.shape(0);
  int * data_vertices_from = vertices_from.mutable_data();
  int * data_vertices_to = vertices_to.mutable_data();
  scalar_t * data_weights = weights.mutable_data();

  ksp = new Ksp(n_vertices, n_edges, data_vertices_from,
                data_vertices_to, data_weights, source, sink);

}
PyKsp::~PyKsp(){

  ksp->~Ksp();
}

std::vector<py::array_t<int>> PyKsp::run(){
  return ksp->run();
}

PYBIND11_MODULE(pyksp, m) {
    py::class_<PyKsp>(m, "PyKsp")
      .def(py::init<const py::array_t<int> &,
           const py::array_t<int> &,
           const py::array_t<scalar_t> &,
           const int &, const int &, const int &>())
      .def("config", &PyKsp::config, "Set parameters",
      py::arg("tol") = 1e-6, py::arg("min_cost") = false, py::arg("verbose") = true, py::arg("l_max") = -1)
      .def("run", &PyKsp::run, "Runs K-shortest paths")
      .def("__repr__",
              [](const PyKsp &a) {
                  return "<pyksp.PyKsp '" + a.summary() + "'>";
              });
}

