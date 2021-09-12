#define FORCE_IMPORT_ARRAY
#include <xtensor-python/pyarray.hpp>     // Numpy bindings
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
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
    .def_readonly("name", &T::name)
    .def_property_readonly("state", &T::getState, py::return_value_policy::reference)
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
using cairl::envs::CartPoleEnv;
using cairl::envs::AcrobotEnv;
using cairl::envs::PendulumEnv;
using cairl::envs::MountainCarContinuousEnv;
using cairl::envs::MountainCarEnv;
using cairl::contrib::DeepRTS::DeepRTSGoldCollectFifteen;
using cairl::contrib::DeepRTS::DeepRTSLavaMaze;
using cairl::contrib::DeepRTS::DeepRTSOneVersusOne;


using cairl::envs::flashrl::MultitaskEnv;

void init_environments(py::module& m)
{
    auto classic_control_module = m.def_submodule("classic_control");

    bind_environment<CartPoleEnv>(classic_control_module, "CartPoleEnv", {})
            .def(py::init<>())
            .def("__override_state", &CartPoleEnv::override_state);

    bind_environment<AcrobotEnv>(classic_control_module, "AcrobotEnv", {})
            .def(py::init<>())
            .def("__override_state", &AcrobotEnv::override_state);

    bind_environment<PendulumEnv>(classic_control_module, "PendulumEnv", {})
            .def(py::init<>())
            .def("__override_state", &PendulumEnv::override_state);

    bind_environment<MountainCarContinuousEnv>(classic_control_module, "MountainCarContinuousEnv", {})
            .def(py::init<>())
            .def("__override_state", &MountainCarContinuousEnv::override_state);

    bind_environment<MountainCarEnv>(classic_control_module, "MountainCarEnv", {})
            .def(py::init<>())
            .def("__override_state", &MountainCarEnv::override_state);

    auto flashrl = m.def_submodule("flashrl");
    bind_environment<FlashEnv>(flashrl, "FlashEnv", {})
    .def(py::init<const std::string&, const std::string&>());

    bind_environment<MultitaskEnv>(flashrl, "MultitaskEnv", {})
    .def(py::init<>());


    auto deeprts = m.def_submodule("deeprts");

    bind_environment<DeepRTSGoldCollectFifteen>(deeprts, "DeepRTSGoldCollectFifteen", {})
    .def(py::init<>());
    bind_environment<DeepRTSLavaMaze>(deeprts, "DeepRTSLavaMaze", {})
    .def(py::init<>());
    bind_environment<DeepRTSOneVersusOne>(deeprts, "DeepRTSOneVersusOne", {})
    .def(py::init<>());



}
