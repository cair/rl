//
// Created by per on 7/28/21.
//

#ifndef CLASSIC_CONTROL_BOX_H
#define CLASSIC_CONTROL_BOX_H


#include "Space.h"
#include <fmt/ostream.h>
#include <array>
#include "cairl/utils/VectorWrapper.h"
#include <utility>
#include <xtensor/xarray.hpp>

namespace cairl::spaces{


class Box: public Space<
        xt::xarray<double, xt::layout_type::row_major>,
        std::vector<std::size_t>>{
        private:
            const std::vector<std::size_t> shape;


        public:
    [[nodiscard]] DataInitReturnType initialize() const override{
        return shape;
    }

    Box(std::vector<std::size_t> shape)
        : shape(std::move(shape))
        {}


        [[nodiscard]] DataType sample() override {
            // https://github.com/openai/gym/blob/31be35ecd460f670f0c4b653a14c9996b7facc6c/gym/spaces/box.py#L83
            return DataType(shape);
        }


        [[nodiscard]] constexpr bool contains(const DataType& x) const override{
            return true; // TODO - xtensor max and min.
            //return std::min(x) >= low && std::max(x) <= high;
        }
};

}


#endif //CLASSIC_CONTROL_BOX_H
