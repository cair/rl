//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_SPACE_H
#define CLASSIC_CONTROL_SPACE_H
#include <effolkronium/random.hpp>
#include <ctime>

namespace cairl::spaces {

    template <class DataTypeT, typename DataInitReturnTypeT>
    class Space{

    public:
        using DataType =  DataTypeT;
        using DataInitReturnType = DataInitReturnTypeT;



        effolkronium::random_local rng;

        constexpr Space() = default;

        [[nodiscard]] constexpr virtual bool contains(const DataType& x) const { return false;}

        virtual DataInitReturnType initialize() const = 0;

        virtual DataType sample()  = 0;

        constexpr void seed(uint64_t seed_){

            rng.seed(seed_);
        }

    };

    // TODO - IN Pybind you should:
    // TODO implement __repr__
    // TODO implement __eq__
    // TODO implement to_jsonable https://github.com/openai/gym/blob/31be35ecd460f670f0c4b653a14c9996b7facc6c/gym/spaces/box.py#L133
    // TODO implement from_jsonable


};

#endif //CLASSIC_CONTROL_SPACE_H
