//
// Created by per on 7/26/21.
//

#ifndef FLASHRL_ENV_H
#define FLASHRL_ENV_H
#include "cairl/environments/Env.h"
#include "cairl/spaces/Box.h"
#include "cairl/spaces/Discrete.h"
#include "cairl/environments/flashrl/lightspark/LightsparkRunner.h"


using cairl::spaces::Space;


namespace cairl::envs::flashrl{

    class FlashEnv: public Env<Discrete, Box, uint8_t , 80, 80, 3>{
    cv::Mat dummy;
    StateType state;
    public:

        LightsparkRunner runner;


        explicit FlashEnv(const std::string& url)
        : Env({20}, {0, 255})
        , runner(url, 1000)
        {}

        const cv::Mat& render(const char* = "human"/*mode*/) override{
            SPDLOG_ERROR("render() function is not implemented in FlashEnv!");
            return dummy;
        }


        StepReturnType step(ActionType a) override{
            // n = action value

            runner.ed->cond.wait(runner.ed->lk);
            cv::Mat output;
            {
                std::scoped_lock g(runner.ed->m);
                cv::resize(runner.ed->frame, output, cv::Size(150, 150), cv::INTER_LINEAR);
            }

            state = mat_to_xarray(output); //to_arma<uchar, 3>(output);
            terminal = false;
            reward = 0.0;

            return std::tie(state, reward, terminal, info);
        }

        StateType& reset() override{
            runner.start();
            return state;
        }

};


} // namespace cairl::envs


#endif //FLASHRL_ENV_H
