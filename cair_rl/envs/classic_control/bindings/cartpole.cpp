
#include "../include/envs/classic_control/cartpole/cartpole.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "converters/ndarray_converter.h"

namespace py = pybind11;

void init_cartpole(py::module& m)
{

    py::class_<CartPoleEnv>(m, "CartPoleEnv")
            .def(py::init())
            .def_readonly("action_space", &CartPoleEnv::action_space)
            .def_readonly("state", &CartPoleEnv::state, py::return_value_policy::reference)
            .def("render", &CartPoleEnv::render, py::return_value_policy::reference)
            .def("reset", &CartPoleEnv::reset, py::return_value_policy::reference)
            .def("__override_state", &CartPoleEnv::override_state)
            .def("step", &CartPoleEnv::step);
}
