//
// Created by per on 9/10/21.
//

#ifndef CLASSIC_CONTROL_DEEPRTS_H
#define CLASSIC_CONTROL_DEEPRTS_H
#include "cairl/environments/Env.h"
#include <DeepRTS/scenario/scenarios/GoldCollectFifteen.h>
#include <DeepRTS/scenario/scenarios/GeneralAIOneVersusOne.h>
#include <DeepRTS/scenario/scenarios/LavaMaze.h>
#include <DeepRTS/Constants.h>
#include <opencv2/core.hpp>

using DeepRTS::Scenarios::GoldCollectFifteen;
using DeepRTS::Scenarios::GeneralAIOneVersusOne;
using DeepRTS::Scenarios::LavaMaze;
using DeepRTS::Constants::ACTION_MAX;
namespace cairl::contrib::DeepRTS{

    template <class DeepRTSCollectionType>
    class DeepRTSCollection: public Env<Discrete, Box, uint8_t , 80, 80, 3>{
        cv::Mat dummy;
        StateType state;

    public:


        DeepRTSCollectionType game;

        explicit DeepRTSCollection(const std::string& envName, const std::unordered_map<std::string, std::string>& config = {})
                : Env(envName, config, {ACTION_MAX}, {0, 255})
                , game()
        {
            game.setMaxFPS(0);
            game.start();
        }

        const cv::Mat& render(const char* = "human"/*mode*/) override{
            cv::resize(game.render(), dummy, cv::Size(80, 80));
            return dummy;
        }


        StepReturnType step(ActionType a) override{
            // n = action value
            auto action = a + 1;
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


    class DeepRTSGoldCollectFifteen : public DeepRTSCollection<GoldCollectFifteen>{
    public:
        DeepRTSGoldCollectFifteen(const std::unordered_map<std::string, std::string>& config = {}): DeepRTSCollection<GoldCollectFifteen>("DeepRTSGoldCollectFifteen-v0", config){
            game.addPlayer();
        }
    };


    class DeepRTSLavaMaze : public DeepRTSCollection<LavaMaze>{
        public:
        DeepRTSLavaMaze(const std::unordered_map<std::string, std::string>& config = {}): DeepRTSCollection<LavaMaze>("DeepRTSLavaMaze-v0", config){
            game.addPlayer();
        }
    };


    class DeepRTSOneVersusOne : public DeepRTSCollection<GeneralAIOneVersusOne>{
    public:
        DeepRTSOneVersusOne(const std::unordered_map<std::string, std::string>& config = {}): DeepRTSCollection<GeneralAIOneVersusOne>("DeepRTSOneVersusOne-v0", config){
            game.addPlayer();
            game.addPlayer();
        }
    };


}



#endif //CLASSIC_CONTROL_DEEPRTS_H
