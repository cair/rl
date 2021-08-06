//
// Created by per on 8/6/21.
//

#ifndef CLASSIC_CONTROL_VECTORWRAPPERCONVERTER_H
#define CLASSIC_CONTROL_VECTORWRAPPERCONVERTER_H
#include "cairl/utils/VectorWrapper.h"
#include <pybind11/pybind11.h>
namespace py = pybind11;

using cairl::utils::VectorWrapper;

namespace pybind11::detail {

    template <class Dtype>
            struct type_caster<VectorWrapper<Dtype>> : public type_caster_base<VectorWrapper<Dtype>>{
                using base = type_caster_base<VectorWrapper<Dtype>>;
            public:
                bool load(handle src, bool convert) {
                    if (base::load(src, convert)) {
                        std::cerr << "loaded via base!\n";
                        return true;
                    }
                    else if (py::isinstance<py::int_>(src)) {
                        std::cerr << "loading from integer!\n";
                        //value = new VectorWrapper<Dtype>(py::cast<int>(src));
                        return true;
                    }

                    return false;
                }

                static handle cast(const VectorWrapper<Dtype>& src, return_value_policy policy, handle parent) {
                    /* Do any additional work here */
                    const auto& data = src.data();
                    return py::array_t<Dtype>(
                            /*shape=*/{data.size()},
                            /*strides=*/{sizeof(Dtype)},
                            /*ptr=*/data.data()
                            ).release();
                }

                static handle cast(VectorWrapper<Dtype>& src, return_value_policy policy, handle parent) {
                    /* Do any additional work here */
                    auto& data = src.data();
                    return py::array_t<Dtype>(
                            /*shape=*/{data.size()},
                            /*strides=*/{sizeof(Dtype)},
                            /*ptr=*/data.data()
                            ).release();
                }
            };

}

#endif //CLASSIC_CONTROL_VECTORWRAPPERCONVERTER_H
