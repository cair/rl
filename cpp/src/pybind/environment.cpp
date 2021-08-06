#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <carma>

#include "cairl/envs.h"


#include "converters/ndarray_converter.h"
#include "converters/VectorWrapperConverter.h"

namespace py = pybind11;

template <class T>
py::class_<T> bind_environment(py::module& m, const char* name, std::forward_list<T> args...){
    using VType = typename T::StateType;
    using VDType = typename T::ObservationSpaceType;

    return py::class_<T>(m, name)
    .def_readonly("action_space", &T::action_space)
    .def_property_readonly("state", &T::getState, py::return_value_policy::automatic)
    .def("render", &T::render, py::return_value_policy::reference)
    .def("reset", &T::reset, py::return_value_policy::reference)
    .def("step", &T::step)
    .def("step", [&](T& self, int a){
        return self.step(a);
    });
}


using cairl::envs::CartPoleEnv;
using cairl::envs::flashrl::FlashEnv;
using cairl::envs::Env;
void init_environments(py::module& m)
{
    auto classic_control_module = m.def_submodule("classic_control");

    bind_environment<CartPoleEnv>(classic_control_module, "CartPoleEnv", {})
    .def(py::init<>())
    .def("__override_state", &CartPoleEnv::override_state);


    auto flashrl = m.def_submodule("flashrl");
    bind_environment<FlashEnv>(flashrl, "FlashEnv", {})
    .def(py::init<const std::string&>());


}
