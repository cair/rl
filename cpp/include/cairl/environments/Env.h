//
// Created by per on 7/26/21.
//

#ifndef CLASSIC_CONTROL_ENV_H
#define CLASSIC_CONTROL_ENV_H
#include <limits>
#include <utility>
#include "cairl/spaces/Space.h"
#include "cairl/defs.h"
#include "cairl/utils/xtensor_cv_convertions.h"
using cairl::spaces::Space;

namespace cairl::envs{

    template <class ActionSpaceT, class ObservationSpaceT>
    class Env{
    public:

        /// Box, Discrete or similar space type
        using ObservationSpace = ObservationSpaceT;
        using StateType = typename ObservationSpace::DataType;
        using ActionType = typename ActionSpaceT::DataType;

        using InfoType = std::unordered_map<std::string, std::string>;
        using RewardType = double;
        using TerminalType = bool;
        using StepReturnType = std::tuple<StateType&, RewardType&, TerminalType&, InfoType&>;

    protected:
        cv::Mat dummy;
        effolkronium::random_local rng{};

        /// Contains return data.
        /// We use this as a member variable so that we can pass along a reference.
        InfoType info;
        RewardType reward{};
        TerminalType terminal{};

    public:
        StateType state;
        ActionSpaceT action_space;
        ObservationSpace observation_space;
        const std::string name;
        const std::unordered_map<std::string, std::string> config;

        Env(
                std::string name,
                std::unordered_map<std::string, std::string> config,
                const ActionSpaceT&& action_space,
                const ObservationSpace&& observation_space)
                : name(std::move(name))
                , state(observation_space.initialize())
                , config(std::move(config))
                , action_space(std::move(action_space))
                , observation_space(std::move(observation_space))
                {
            seed(time(NULL));
        }

        void seed(const int seed_){
            rng.seed(seed_);
            action_space.seed(seed_);
            observation_space.seed(seed_);
        }

        virtual const cv::Mat& render(const char*/*mode*/){ return dummy; }
        virtual StepReturnType step(ActionType a) = 0;
        virtual StateType& reset() = 0;

        const StateType& getState() const{
            return state;
        }

    };


} // namespace cairl::envs


#endif //CLASSIC_CONTROL_ENV_H
