//
// Created by per on 7/28/21.
//

#ifndef CLASSIC_CONTROL_BOX_H
#define CLASSIC_CONTROL_BOX_H


#include "Space.h"
#include <fmt/ostream.h>
#include <array>
#include "cairl/utils/VectorWrapper.h"
#include <xtensor/xfixed.hpp>

namespace cairl::spaces{

//#define DATATYPE_MACRO \
//    typename std::conditional<Dims == 1 && Cols == 1, xt::xtensor_fixed<Scalar, xt::xshape<Rows>, xt::layout_type::row_major>,\
//    typename std::conditional<Dims == 1 && Cols >= 2, xt::xtensor_fixed<Scalar, xt::xshape<Rows, Cols>, xt::layout_type::row_major>,\
//    typename std::conditional<Dims >= 2, xt::xtensor_fixed<Scalar, xt::xshape<Rows, Cols, Dims>, xt::layout_type::row_major>, int\
//    >::type\
//    >::type\
//    >::type\

template <typename Scalar, int Rows, int Cols, int Dims>
class Box: public Space<xt::xarray<Scalar, xt::layout_type::row_major>, typename xt::xarray<Scalar>::shape_type>{
        public:
            using DataType = xt::xarray<Scalar, xt::layout_type::row_major>;
            using HighLowType = std::array<std::array<Scalar, 2>, Rows>;
        private:
            const double low;
            const double high;
            const std::array<int, 3> shape = {Rows, Cols, Dims};



        public:


        constexpr typename xt::xarray<Scalar>::shape_type initialize() const override{
            if constexpr(Dims == 1 && Cols == 1){
                return {Rows};
            }else if constexpr(Dims == 1 && Cols > 1){
                return {Rows, Cols};
            }else if constexpr(Dims > 1){
                return {Rows, Cols, Dims};
            }
        }


            constexpr Box(
                    const Scalar low_,
                    const Scalar high_)
            : low(low_)
            , high(high_)
            {
                //static_assert(Rows + Cols > 2); // You cannot initiate a box of shape={1, 1}
            }

            // TODO- thiswhole class needs abit rework in regards to min-max limits


            constexpr Box(std::initializer_list<HighLowType> dataType)
                    : low(0)
                    , high(0)
            {
                // : high_low((empty(dataType)) ? *{} : *dataType.begin())
            }

            constexpr DataType sample() const override {
                // https://github.com/openai/gym/blob/31be35ecd460f670f0c4b653a14c9996b7facc6c/gym/spaces/box.py#L83
                if constexpr(Dims == 1){
                    return DataType({Rows, Cols});
                }else if constexpr(Dims == 1 && Cols == 1){
                    return DataType({Rows});
                }
                return DataType({Rows, Cols, Dims});
            }

            [[nodiscard]] constexpr bool contains(const DataType& x) const override{
                return true; // TODO - xtensor max and min.
                //return std::min(x) >= low && std::max(x) <= high;
            }
        };

}


#endif //CLASSIC_CONTROL_BOX_H
