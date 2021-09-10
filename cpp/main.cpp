#include <iostream>
#include <opencv4/opencv2/highgui.hpp>

#include <spdlog/spdlog.h>
#include "cairl/envs.h"
#include "cairl/spaces/Discrete.h"
#include "cairl/spaces/Box.h"
#include "cairl/wrappers/TimeLimit.h"
#include "cairl/wrappers/Flatten.h"
int main() {
//  auto enc = cairl::envs::flashrl::FlashEnv("https://flashrl.sysx.no/357085_heartsNoAccount.swf");


    using cairl::spaces::Box;
    using cairl::wrappers::TimeLimit;
    using cairl::wrappers::Flatten;
    using cairl::envs::CartPoleEnv;
    using cairl::envs::AcrobotEnv;
    using cairl::envs::PendulumEnv;
    auto env =  PendulumEnv(); //Flatten<TimeLimit<20, CartPoleEnv>>();
    for(int episode = 0; episode < 10000; episode++){
        env.reset();
        int terminal, steps = 0;
        while(true){
            steps++;
            const auto [s1, r, t, info] = env.step(env.action_space.sample());
            //auto obs = env.render();
            terminal = t;
        }
    }

//    cv::imshow("Window", img);
//    cv::waitKey(1);

//    cairl::spaces::Box<double, 2, 2, 1> s{
//            {
//                    {0.0, 0.0},
//                    {0.0, 0.0},
//                    {0.0, 0.0},
//                    {0.0, 0.0}
//            }
//    };
//    auto discrete = cairl::spaces::Discrete(2);
//    SPDLOG_INFO("{}", discrete.sample());

    return 0;
}

