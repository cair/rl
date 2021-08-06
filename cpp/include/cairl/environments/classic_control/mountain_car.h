//
// Created by per on 7/28/21.
//

#ifndef CLASSIC_CONTROL_MOUNTAIN_CAR_H
#define CLASSIC_CONTROL_MOUNTAIN_CAR_H

//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_CARTPOLE_H
#define CLASSIC_CONTROL_CARTPOLE_H
#include <cmath>
#include <limits>
#include "envs/Env.h"
#include <effolkronium/random.hpp>
#include <spaces/Discrete.h>
#include <spdlog/spdlog.h>
#include "rendering/renderer.h"


using Random = effolkronium::random_static;
using cairl::classic_control::renderer::Rectangle;
using cairl::classic_control::renderer::Circle;


class MountainCarEnv: public Env{
//    """
//    Description:
//            The agent (a car) is started at the bottom of a valley. For any given
//            state the agent may choose to accelerate to the left, right or cease
//            any acceleration.
//    Source:
//            The environment appeared first in Andrew Moore's PhD Thesis (1990).
//    Observation:
//            Type: Box(2)
//    Num    Observation               Min            Max
//    0      Car Position              -1.2           0.6
//    1      Car Velocity              -0.07          0.07
//    Actions:
//            Type: Discrete(3)
//    Num    Action
//    0      Accelerate to the Left
//    1      Don't accelerate
//    2      Accelerate to the Right
//    Note: This does not affect the amount of velocity affected by the
//    gravitational pull acting on the car.
//    Reward:
//            Reward of 0 is awarded if the agent reached the flag (position = 0.5)
//    on top of the mountain.
//    Reward of -1 is awarded if the position of the agent is less than 0.5.
//    Starting State:
//            The position of the car is assigned a uniform random value in
//    [-0.6 , -0.4].
//    The starting velocity of the car is always assigned to 0.
//    Episode Termination:
//            The car position is more than 0.5
//    Episode length is greater than 200
//    """



    enum Actions{
        ACCELERATE_LEFT,
        DONT_ACCELERATE
        ACCELERATE_RIGHT
        COUNT
    };

    // refer to https://github.com/openai/gym/blob/master/gym/envs/classic_control/cartpole.py for comments
    static constexpr double min_position = -1.2
    static constexpr double max_position = 0.6;
    static constexpr double max_speed = 0.07;
    static constexpr double goal_position = 0.5;
    static constexpr double goal_velocity_default = 0;
    static constexpr double goal_velocity = goal_velocity_default;

    static constexpr double force = 0.001;
    static constexpr double gravity = 0.0025;

    static constexpr std::array<double> low = {
            min_position, -max_speed,
    };
    static constexpr std::array<double> high = {
            max_position, max_speed,
    };

    /// Rendering
    static constexpr int screen_width = 600;
    static constexpr int screen_height = 400;
    static constexpr double world_width = x_threshold * 2;
    static constexpr double scale = screen_width / world_width;

    static constexpr double carty = 100;
    static constexpr double polewidth = 10.0;
    static constexpr double polelen = scale * ( 2 * length);
    static constexpr int cartwidth = 50;
    static constexpr int cartheight = 30;

    const cairl::spaces::Discrete action_space{Actions::COUNT};

    cairl::classic_control::renderer::Renderer renderer{screen_width, screen_height};

public:

    MountainCarEnv(){
        reset();
    }

    const cv::Mat& render(const char* = "human"/*mode*/){

    }


    StepReturnType step(int action){

        return {
                state,
                reward,
                done,
                {}
        };
    }



    /* Used to validate against openai GYM */
    void override_state(double x, double x_dot, double theta, double theta_dot){
        reset();
        state[0] = x;
        state[1] = x_dot;
        state[2] = theta;
        state[3] = theta_dot;
    }




    std::array<double, 4> reset(){
        state[0] = Random::get<std::uniform_real_distribution<>>(-0.05, 0.05);
        state[1] = Random::get<std::uniform_real_distribution<>>(-0.05, 0.05);
        state[2] = Random::get<std::uniform_real_distribution<>>(-0.05, 0.05);
        state[3] = Random::get<std::uniform_real_distribution<>>(-0.05, 0.05);
        steps_beyond_done = 0;
        return state;
    }





};

#endif //CLASSIC_CONTROL_CARTPOLE_H



#endif //CLASSIC_CONTROL_MOUNTAIN_CAR_H
