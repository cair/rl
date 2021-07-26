//
// Created by per on 7/26/21.
//

#ifndef CLASSIC_CONTROL_ENV_H
#define CLASSIC_CONTROL_ENV_H
#include <limits>
#include <array>
#include "spaces/Space.h"
class Env{

    std::array<double, double> reward_range = {
            std::numeric_limits<double>::min(),
            std::numeric_limits<double>::max()
    };
public:
    const Space action_space;
    const Space observation_space;


};

#endif //CLASSIC_CONTROL_ENV_H
