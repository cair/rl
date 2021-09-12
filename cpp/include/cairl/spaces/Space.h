//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_SPACE_H
#define CLASSIC_CONTROL_SPACE_H
#include <effolkronium/random.hpp>
#include <ctime>

namespace cairl::spaces {




    template <typename DataType, typename DataInitReturnType>
    class Space{

    protected:
        using rng = effolkronium::random_static;


    public:
        constexpr Space()
        {}

        [[nodiscard]] constexpr virtual bool contains(const DataType& x) const { return false;}

        constexpr virtual DataInitReturnType initialize() const = 0;

        constexpr virtual DataType sample() const  = 0;

        constexpr void seed(uint64_t seed_){
            //urng = Eigen::Rand::Vmt19937_64{ static_cast<uint64_t>(seed_) };
            rng::seed(seed_);
        }

    };

    // TODO - IN Pybind you should:
    // TODO implement __repr__
    // TODO implement __eq__
    // TODO implement to_jsonable https://github.com/openai/gym/blob/31be35ecd460f670f0c4b653a14c9996b7facc6c/gym/spaces/box.py#L133
    // TODO implement from_jsonable


};

#endif //CLASSIC_CONTROL_SPACE_H
