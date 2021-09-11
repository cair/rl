//
// Created by per on 9/10/21.
//

#ifndef CLASSIC_CONTROL_MOUNTAINCAR_CONTINUOUS_H
#define CLASSIC_CONTROL_MOUNTAINCAR_CONTINUOUS_H
#include "cairl/defs.h"
#include "cairl/spaces/Discrete.h"
#include "cairl/spaces/Box.h"
#include <cmath>
#include "cairl/environments/Env.h"
#include <xtensor/xio.hpp>
using Random = effolkronium::random_static;
using cairl::spaces::Discrete;
using cairl::spaces::Box;
using cairl::envs::Env;

namespace cairl::envs{


// REFER TO https://github.com/openai/gym/blob/master/gym/envs/classic_control/mountain_car.py
    using cairl::spaces::Space;



//    Description:
//    The agent (a car) is started at the bottom of a valley. For any given
//            state the agent may choose to accelerate to the left, right or cease
//            any acceleration.
//    Observation:
//    Type: Box(2)
//    Num    Observation               Min            Max
//    0      Car Position              -1.2           0.6
//    1      Car Velocity              -0.07          0.07
//    Actions:
//    Type: Box(1)
//    Num    Action                    Min            Max
//    0      the power coef            -1.0           1.0
//    Note: actual driving force is calculated by multipling the power coef by power (0.0015)
//    Reward:
//    Reward of 100 is awarded if the agent reached the flag (position = 0.45) on top of the mountain.
//    Reward is decrease based on amount of energy consumed each step.
//    Starting State:
//    The position of the car is assigned a uniform random value in
//    [-0.6 , -0.4].
//    The starting velocity of the car is always assigned to 0.
//    Episode Termination:
//    The car position is more than 0.45
//    Episode length is greater than 200
    class MountainCarContinuousEnv: public Env<Box<double, 1, 1, 1>, Box, double, 2, 1, 1>
    {

        static constexpr double MIN_ACTION = -1.0;
        static constexpr double MAX_ACTION = 1.0;

        static constexpr double MIN_POSITION = -1.2;
        static constexpr double MAX_POSITION = 0.6;
        static constexpr double MAX_SPEED = 0.07;
        static constexpr double GOAL_POSITION = 0.45; // # was 0.5 in gym, 0.45 in Arnaud de Broissia's version
        static constexpr double GOAL_VELOCITY = 0.0; // todo - input argument? https://github.com/openai/gym/blob/master/gym/envs/classic_control/mountain_car.py#L57
        static constexpr double POWER = 0.0015;
        static constexpr double GRAVITY = 0.0025;

    public:


        MountainCarContinuousEnv()
                : Env({},{
                {-1.2, 0.6},
                {-0.07, 0.07},
        })
        {
            reset();
        }

        const cv::Mat& render(const char* = "human"/*mode*/) override{
            // todo - https://github.com/openai/gym/blob/master/gym/envs/classic_control/mountain_car.py#L108
            return dummy;
        }



        static double clip(double value, double min, double max){
            return std::max(std::min(value, max), min);
        }

        StepReturnType step(ActionType as) override {
            auto action = as[0];

            if(action < MIN_ACTION || action > MAX_ACTION){
                throw std::runtime_error(fmt::format("{} ({}) invalid", action, "double"));
            }

            auto position = state[0];
            auto velocity = state[1];
            const auto force = action;
            velocity += force * POWER - GRAVITY * std::cos(3 * position);
            if(velocity > MAX_SPEED){
                velocity = MAX_SPEED;
            }else if(velocity < -MAX_SPEED){
                velocity - MAX_SPEED;
            }
            position += velocity;

            if(position > MAX_POSITION){
                position = MAX_POSITION;
            }else if(position < MIN_POSITION) {
                position = MIN_POSITION;
            }

            if(position == MIN_POSITION &&  velocity < 0){
                velocity = 0;
            }

            terminal = position >= GOAL_POSITION && velocity >= GOAL_VELOCITY;
            reward = (terminal) ? 100.0 : 0.0;
            reward -= std::pow(action, 2) * 0.1;

            state[0] = position;
            state[1] = velocity;

            return std::tie(state, reward, terminal, info);
        }


        /* Used to validate against openai GYM */
        void override_state(double a, double b){
            reset();
            state[0] = a;
            state[1] = b;
        }


        StateType& reset() override{
            state[0] = rng.get<std::uniform_real_distribution<>>(-0.6, -0.4);
            state[1] = 0;
            terminal = false;
            reward = 0.0;
            info = {};
            return state;
        }





    };


} // namespace cairl::envs



#endif //CLASSIC_CONTROL_ACROBOT_H
