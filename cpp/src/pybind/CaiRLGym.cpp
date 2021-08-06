//
// Created by per on 7/25/21.
//

//
// Created by per on 28.01.18.
//

#include <pybind11/pybind11.h>
#include "converters/ndarray_converter.h"

namespace py = pybind11;
void init_cvmat(py::module&);
void init_environments(py::module&);
void init_spaces(py::module&);

void init_version(py::module& m)
{
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

#define INITS \
    init_cvmat(util_module);          \
    init_spaces(spaces_module);          \
    init_environments(envs_module);                 \
    init_version(m);
#define BINDING_INIT \
    auto util_module = m.def_submodule("util"); \
    auto envs_module = m.def_submodule("envs");                 \
    auto spaces_module = m.def_submodule("spaces"); \
    m.doc() = "CaiRL Gym";

#ifndef EMBEDDED
PYBIND11_MODULE(gym, m)
{
    NDArrayConverter::init_numpy();
    BINDING_INIT
    INITS
}
#else
PYBIND11_EMBEDDED_MODULE(gym, m)
{
    BINDING_INIT
    INITS
}
#endif