//
// Created by per on 7/28/21.
//

#ifndef CLASSIC_CONTROL_BOX_H
#define CLASSIC_CONTROL_BOX_H

#include <armadillo>

#include "Space.h"
#include <fmt/ostream.h>
#include <array>
#include "cairl/utils/VectorWrapper.h"
#include <xtensor/xarray.hpp>

namespace cairl::spaces{

    template <typename Scalar, int Cols, int Rows, int Dims>
    class Box: public Space<xt::xarray<Scalar, xt::layout_type::row_major>>{
    public:
        using DataType = xt::xarray<Scalar, xt::layout_type::row_major>;
        using HighLowType = std::array<std::array<Scalar, 2>, Rows>;
    private:
        const double low;
        const double high;
        const HighLowType high_low;


        const std::array<int, 3> shape = {Rows, Cols, Dims};
    public:


        constexpr Box(
                const Scalar low_,
                const Scalar high_)
        : low(low_)
        , high(high_)
        , high_low()
        {
            //static_assert(Rows + Cols > 2); // You cannot initiate a box of shape={1, 1}
        }


        constexpr Box(std::initializer_list<HighLowType> dataType)
                : high_low(*dataType.begin())
                , low(0)
                , high(0)
        {}
        constexpr DataType sample() override {
            // https://github.com/openai/gym/blob/31be35ecd460f670f0c4b653a14c9996b7facc6c/gym/spaces/box.py#L83
            if constexpr(Dims == 1){
                return DataType({Cols, Rows});
            }else if constexpr(Dims == 1 && Rows == 1){
                return DataType({Cols});
            }
            return DataType({Cols, Rows, Dims});
        }

        [[nodiscard]] constexpr bool contains(const DataType& x) const override{
            return true; // TODO - xtensor max and min.
            //return std::min(x) >= low && std::max(x) <= high;
        }
    };

}


#endif //CLASSIC_CONTROL_BOX_H
