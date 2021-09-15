//
// Created by per on 9/10/21.
//

#ifndef CLASSIC_CONTROL_PENDULUM_H
#define CLASSIC_CONTROL_PENDULUM_H
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


    class PendulumEnv: public Env<Box, Box> {

        static constexpr double MAX_SPEED = 8.0;
        static constexpr double MAX_TORQUE = 2.0;

        static constexpr double DT = 0.05;
        static constexpr double G = 10.0;
        static constexpr double M = 1.0;
        static constexpr double L = 1.0;

        StateType state2;
    public:


        PendulumEnv()
            : Env(
                "Pendulum-v3",
                {},
                {{1, 1, 1}},
                {{3, 1, 1}}
        ){
            reset();
        }

        const cv::Mat &render(const char * = "human"/*mode*/) override {
            // todo - https://github.com/openai/gym/blob/master/gym/envs/classic_control/pendulum.py
            return dummy;
        }


        static double angle_normalize(double x) {
            return (std::fmod((x + M_PI), (2 * M_PI))) - M_PI;
        }


        static double clip(double value, double min, double max) {
            return std::max(std::min(value, max), min);
        }

        StepReturnType step(ActionType as) override {
            auto action = as[0];

            if (action < -MAX_TORQUE || action > MAX_TORQUE) {
                throw std::runtime_error(fmt::format("{} ({}) invalid", action, "double"));
            }

            const auto th = state[0];
            const auto thdot = state[1];


            const auto costs =
                    std::pow(angle_normalize(th), 2) + 0.1 * std::pow(thdot, 2) + 0.001 * (std::pow(action, 2));
            auto newthdot = (
                    thdot
                    + (-3 * G / (2 * L) * std::sin(th + M_PI) + 3.0 / (M * std::pow(L, 2)) * action) * DT
            );
            auto newth = th + newthdot * DT;
            newthdot = clip(newthdot, -MAX_SPEED, MAX_SPEED);

            state[0] = newth;
            state[1] = newthdot;
            reward = -costs;
            terminal = false;

            state2[0] = std::cos(state[0]);
            state2[1] = std::sin(state[0]);
            state2[2] = state[1];

            return std::tie(state2, reward, terminal, info);
        }


        /* Used to validate against openai GYM */
        void override_state(double a, double b) {
            reset();
            state[0] = a;
            state[1] = b;
        }


        StateType &reset() override {
            state[0] = rng.get<std::uniform_real_distribution<>>(-M_PI, M_PI);
            state[1] = rng.get<std::uniform_real_distribution<>>(-1.0, 1.0);
            terminal = false;
            reward = 0.0;
            info = {};
            return state;
        }

    };



}
#endif //CLASSIC_CONTROL_PENDULUM_H
