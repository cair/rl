//
// Created by per on 7/25/21.
//
#include "../include/spaces/Space.h"
#include "../include/spaces/Discrete.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <any>
namespace py = pybind11;
using cairl::spaces::Space;
using cairl::spaces::Discrete;

class PySpace : public cairl::spaces::Space {

    using cairl::spaces::Space::Space;


    [[nodiscard]] bool contains(const std::any& x) const  override
    {
        PYBIND11_OVERLOAD_PURE(bool, Space, contains, x)
    }

};


void init_spaces(py::module& m) {

    py::class_<Space, PySpace>(m, "Space")
            .def(py::init())
            .def("contains", &Space::contains);

    py::class_<Discrete>(m, "Discrete")
            .def(py::init<int>())
            .def_readonly("n", &Discrete::n)
            .def("sample", &Discrete::sample)
            .def("contains", &Discrete::contains);
}
