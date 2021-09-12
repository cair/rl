//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_DISCRETE_H
#define CLASSIC_CONTROL_DISCRETE_H
#include <any>
#include <effolkronium/random.hpp>
#include <cairl/spaces/Space.h>
using Random = effolkronium::random_static;
using cairl::spaces::Space;

namespace cairl::spaces{


    class Discrete: public Space<int, int>{


    public:
        using DataType = int;
        const int n;

        [[nodiscard]] constexpr int initialize() const override{
            return 0;
        }


        constexpr Discrete(const int n_)
        : n(n_)
        {}

        operator int() const { return n; }


        [[nodiscard]] int sample() const override{
            return rng::get(0, n-1);
        }

        [[nodiscard]] bool contains(const int& x) const override{
            return x >= 0 && x < n;
        }


    };

} // namespace cairl::spaces


#endif //CLASSIC_CONTROL_DISCRETE_H
