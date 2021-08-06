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

namespace cairl::spaces{


#define DATATYPE_MACRO \
    typename std::conditional<Dims == 1 && Rows == 1, cairl::utils::VectorWrapper<Scalar>,\
    typename std::conditional<Dims == 1 && Rows >= 2, arma::Mat<Scalar>,\
    typename std::conditional<Dims >= 2, arma::Cube<Scalar>, int\
    >::type\
    >::type\
    >::type\


    template <typename Scalar, int Cols, int Rows, int Dims>
    class Box: public Space<DATATYPE_MACRO>{
    public:
        using DataType = DATATYPE_MACRO;
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
        {


           // static_assert(Rows + Cols > 2); // You cannot initiate a box of shape={1, 1}
        }
        constexpr DataType sample() override {
            // https://github.com/openai/gym/blob/31be35ecd460f670f0c4b653a14c9996b7facc6c/gym/spaces/box.py#L83
            // https://bab2min.github.io/eigenrand/v0.3.5/en/list_of_supported_distribution.html
            SPDLOG_INFO("Unimplemented sample type.");
            if constexpr(std::is_same_v<DataType, arma::Cube<Scalar>>){
                return DataType (Cols, Rows, Dims);
            }else if constexpr(std::is_same_v<DataType, arma::Mat<Scalar>>){
                return DataType (Cols, Rows);
            }else if constexpr(std::is_same_v<DataType, cairl::utils::VectorWrapper<Scalar>>){
                return DataType(Cols);
            }
        }

        [[nodiscard]] constexpr bool contains(const DataType& x) const override{
            return x.min() >= low && x.max() <= high;
        }
    };

}


#endif //CLASSIC_CONTROL_BOX_H
