//
// Created by per on 9/10/21.
//

#ifndef CLASSIC_CONTROL_DEEPRTS_H
#define CLASSIC_CONTROL_DEEPRTS_H
#include "cairl/environments/Env.h"
#include <DeepRTS/scenario/scenarios/GoldCollectFifteen.h>
#include <DeepRTS/Constants.h>
#include <opencv2/core.hpp>

using DeepRTS::Scenario::GoldCollectFifteen;
using DeepRTS::Constants::ACTION_MAX;
namespace cairl::contrib::DeepRTS{

    class DeepRTSGoldCollectFifteen: public Env<Discrete, Box, uint8_t , 80, 80, 3>{
        cv::Mat dummy;
        StateType state;

    public:


        GoldCollectFifteen game;

        explicit DeepRTSGoldCollectFifteen()
                : Env({ACTION_MAX}, {0, 255})
                , game()
        {
            game.addPlayer();
            game.setMaxFPS(0);
            game.start();
        }

        const cv::Mat& render(const char* = "human"/*mode*/) override{
            cv::resize(game.render(), dummy, cv::Size(80, 80));
            return dummy;
        }


        StepReturnType step(ActionType a) override{
            // n = action value
            auto action = a+1;
            // Action
            auto &player = game.players.at(0);
            player.do_action(action);
            game.update();

            // Scenario
            terminal = game.evaluate(player);
            reward = game.reward(player);

            return std::tie(state, reward, terminal, info);
        }

        StateType& reset() override{
            return state;
        }

    };



}



#endif //CLASSIC_CONTROL_DEEPRTS_H
