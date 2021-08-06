#include <iostream>
#include <opencv4/opencv2/highgui.hpp>

#include <spdlog/spdlog.h>
#include "cairl/envs.h"
#include "cairl/spaces/Discrete.h"
#include "cairl/spaces/Box.h"

int main() {
    /*
    auto enc = cairl::envs::flashrl::FlashEnv("https://flashrl.sysx.no/357085_heartsNoAccount.swf");

    int fps = 0;

    TimerClock s2;
    int i = 0;

    for(auto a = 0; a < 100000; a++){
        enc.reset(); // todo
        i = 0;
        while(i < 1000){
            auto [state, reward, terminal, info] = enc.step(0);
            auto s1 = to_cvmat(state);
            cv::imshow("lol", s1);
            cv::waitKey(1);
            fps++;
            if(s2.get_ms() >= 1000){
                SPDLOG_INFO("FPS: {}", fps);
                s2.start();
                fps = 0;
            }
            i++;
        }
    }*/


    cairl::spaces::Box<double, 2, 2, 1> s{
            {
                    {0.0, 0.0},
                    {0.0, 0.0},
                    {0.0, 0.0},
                    {0.0, 0.0}
            }
    };

    auto discrete = cairl::spaces::Discrete(2);
    SPDLOG_INFO("{}", discrete.sample());

    //auto box = cairl::spaces::Box<double, 2, 6>(-2.0, 2.0);
    //SPDLOG_INFO("\n{}", box.sample());

    //auto box2 = cairl::spaces::Box<double, 2, 1>(-2.0, 2.0);
    //SPDLOG_INFO("\n{}", box2.sample());


    auto env = cairl::envs::CartPoleEnv();

    int episodes = 10000;
    for(int episode = 0; episode < episodes; episode++){
        env.reset();
        auto terminal = false;
        int steps = 0;
        while(!terminal){
            steps++;
            const auto [s1, r, t, info] = env.step(env.action_space.sample());
            //auto img = env.render();
            terminal = t;

            //cv::imshow("lol", img);
            //cv::imwrite("test.png", img);
            //cv::waitKey(30);
        }
        //SPDLOG_INFO("Steps: {}", steps);

    }




    return 0;
}

