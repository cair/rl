//
// Created by per on 9/10/21.
//

#ifndef CLASSIC_CONTROL_MOUNTAINCAR_H
#define CLASSIC_CONTROL_MOUNTAINCAR_H
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
//    Source:
//    The environment appeared first in Andrew Moore's PhD Thesis (1990).
//    Observation:
//    Type: Box(2)
//    Num    Observation               Min            Max
//    0      Car Position              -1.2           0.6
//    1      Car Velocity              -0.07          0.07
//    Actions:
//    Type: Discrete(3)
//    Num    Action
//    0      Accelerate to the Left
//    1      Don't accelerate
//    2      Accelerate to the Right
//    Note: This does not affect the amount of velocity affected by the
//    gravitational pull acting on the car.
//    Reward:
//    Reward of 0 is awarded if the agent reached the flag (position = 0.5)
//    on top of the mountain.
//    Reward of -1 is awarded if the position of the agent is less than 0.5.
//    Starting State:
//    The position of the car is assigned a uniform random value in
//    [-0.6 , -0.4].
//    The starting velocity of the car is always assigned to 0.
//    Episode Termination:
//    The car position is more than 0.5
//    Episode length is greater than 200
    class MountainCarEnv: public Env<Discrete, Box, double, 2, 1, 1>
    {
        static constexpr double MIN_POSITION = -1.2;
        static constexpr double MAX_POSITION = 0.6;
        static constexpr double MAX_SPEED = 0.07;
        static constexpr double GOAL_POSITION = 0.5;
        static constexpr double GOAL_VELOCITY = 0.0; // todo - input argument? https://github.com/openai/gym/blob/master/gym/envs/classic_control/mountain_car.py#L57
        static constexpr double FORCE = 0.001;
        static constexpr double GRAVITY = 0.0025;


    public:


        MountainCarEnv()
                : Env({3},{
                {-1.2, 0.6},
                {-0.07, 0.07},
        })
        {
            reset();
        }

        const cv::Mat& render(const char* = "human"/*mode*/) override{
            // todo - https://github.com/openai/gym/blob/master/gym/envs/classic_control/mountain_car.py#L108
            return cv::Mat();
        }



        double clip(double value, double min, double max){
            return std::max(std::min(value, max), min);
        }

        StepReturnType step(ActionType as) override {
            auto action = as;

            if(!action_space.contains(action)){
                throw std::runtime_error(fmt::format("{} ({}) invalid", action, "int"));
            }

            auto position = state[0];
            auto velocity = state[1];
            velocity += (action - 1) * FORCE + std::cos(3 * position) * (-GRAVITY);
            velocity = clip(velocity, -MAX_SPEED, MAX_SPEED);
            position += velocity;
            position = clip(position, MIN_POSITION, MAX_POSITION);
            if(position == MIN_POSITION && velocity < 0){
                velocity = 0;
            }

            terminal = position >= GOAL_POSITION && velocity >= GOAL_VELOCITY;
            reward = (terminal) ? 0.0 : -1.0;

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
