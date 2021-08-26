//
// Created by per on 8/15/21.
//

#ifndef CLASSIC_CONTROL_FLATTEN_H
#define CLASSIC_CONTROL_FLATTEN_H
#include "cairl/environments/Env.h"
#include <xtensor/xarray.hpp>

namespace cairl::wrappers {
    using cairl::envs::Env;

    template<class Environment>
    class Flatten: public Environment{
        using StepReturnType = std::tuple<
                typename Environment::StateType&,
                typename Environment::RewardType&,
                typename Environment::TerminalType&,
                typename Environment::InfoType&>;
        typename Environment::StateType flattened;

    public:

        typename Environment::StepReturnType step(ActionSpace a) override{
            auto tuple =  Environment::step(a);
            flattened = xt::flatten(std::get<0>(tuple));

            return std::tie(flattened, std::get<1>(tuple), std::get<2>(tuple), std::get<3>(tuple));
        }

        typename Environment::StateType& reset() override{
            return Environment::reset();
        }

    };


} // namespace cairl::wrappers

#endif //CLASSIC_CONTROL_FLATTEN_H
