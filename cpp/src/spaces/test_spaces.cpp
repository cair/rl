//
// Created by per on 7/28/21.
//
#include <spdlog/spdlog.h>
//#include "Discrete.h"
#include "Box.h"

int main(){

    //auto x = cairl::spaces::Discrete(5);
    auto z = cairl::spaces::Box<double>(-1, 1, 3);
    auto y = cairl::spaces::Box<std::array<double, 2>>(
            {-1, -2},
            {1, 2},
            {1, 1});
    //SPDLOG_INFO("y={}", y.sample());
    //SPDLOG_INFO("z={}", z.sample());

}