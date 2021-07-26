//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_DISCRETE_H
#define CLASSIC_CONTROL_DISCRETE_H
#include <any>
#include <effolkronium/random.hpp>
#include <spaces/Space.h>
using Random = effolkronium::random_static;
namespace cairl::spaces{

    class Discrete: public cairl::spaces::Space{

    public:
        const int n;

        explicit Discrete(const int n_)
        : Space()
        , n(n_)
        {
            assert(n >= 0);
        }

        [[nodiscard]] int sample() const{
            return Random::get(0, n-1);

        }


        [[nodiscard]] bool contains(const std::any& x) const override{

            if(x.type() != typeid(int)){
                return false;
            }

            const int a = std::any_cast<int>(x);
            return a >= 0 && a < n;



        }


    };

} // namespace cairl::spaces


#endif //CLASSIC_CONTROL_DISCRETE_H
