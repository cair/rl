//
// Created by per on 7/26/21.
//

#ifndef CLASSIC_CONTROL_ENV_H
#define CLASSIC_CONTROL_ENV_H
#include <limits>
#include "cairl/spaces/Space.h"
#include "cairl/defs.h"
using cairl::spaces::Space;

namespace cairl::envs{

    template <
        class ActionSpace,
            template<class, int, int, int> class ObservationSpaceT,
            class StateScalar,
            int StateCols,
            int StateRows,
            int StateDims
    >
    class Env{
    public:

        /// Box, Discrete or similar space type
        using ObservationSpaceType = StateScalar;
        using ObservationSpace = ObservationSpaceT<StateScalar, StateCols, StateRows, StateDims>;
        using StateType = typename ObservationSpace::DataType;

        using InfoType = std::unordered_map<std::string, std::string>;
        using RewardType = double;
        using TerminalType = bool;
        using StepReturnType = std::tuple<StateType&, RewardType&, TerminalType&, InfoType&>;

    private:
        cv::Mat dummy;
        // TODO
        /*std::array<double, 2> reward_range = {
                std::numeric_limits<double>::min(),
                std::numeric_limits<double>::max()
        };*/
    protected:
        effolkronium::random_local rng{};

        /// Contains return data.
        /// We use this as a member variable so that we can pass along a reference.
        InfoType info;
        RewardType reward;
        TerminalType terminal;

    public:
        StateType state;
        ActionSpace action_space;
        ObservationSpace observation_space;

        constexpr Env(const ActionSpace&& action_space_, const ObservationSpace&& observation_space_)
                : action_space(std::move(action_space_))
                , observation_space(std::move(observation_space_)){
            seed(time(NULL));

            if constexpr(std::is_same_v<StateType, arma::Cube<StateScalar>>){
                state = StateType(StateCols, StateRows, StateDims);
            }else if constexpr(std::is_same_v<StateType, arma::Mat<StateScalar>>){
                state = StateType(StateCols, StateRows);
            }else if constexpr(std::is_same_v<StateType, cairl::utils::VectorWrapper<StateScalar>>){
                state = StateType(StateCols);
            }

        }

        void seed(const int seed_){
            rng.seed(seed_);
            action_space.seed(seed_);
            observation_space.seed(seed_);
        }

        virtual const cv::Mat& render(const char*/*mode*/){ return dummy; }
        virtual StepReturnType step(ActionSpace a) = 0;
        virtual StateType& reset() = 0;

        StateType& getState(){
            return state;
        }

    };


} // namespace cairl::envs


#endif //CLASSIC_CONTROL_ENV_H
