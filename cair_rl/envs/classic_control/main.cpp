#include <iostream>
#include <envs/classic_control/cartpole/cartpole.h>

#include <opencv2/highgui.hpp>

int main() {

    auto x = CartPoleEnv();
    int steps = 0;
    int counter = 10000;
    for(int i = 0; i < counter; i++){
        const auto data = x.step(x.action_space.sample());
        const bool done = std::get<2>(data);
        steps++;
        if(done){
            spdlog::info("Steps: {}", steps);
            x.reset();
            steps = 0;
        }
        auto img = x.render();
        //cv::imshow("lol", img);
        //cv::imwrite("test.png", img);
        //cv::waitKey(30);
    }



    return 0;
}

