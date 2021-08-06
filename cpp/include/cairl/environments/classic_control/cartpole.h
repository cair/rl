//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_CARTPOLE_H
#define CLASSIC_CONTROL_CARTPOLE_H
#include <cmath>
#include <limits>
#include <spdlog/spdlog.h>
#include "cairl/defs.h"
#include "cairl/spaces/Discrete.h"
#include "cairl/spaces/Box.h"

#include "cairl/rendering/renderer.h"
#include "cairl/environments/Env.h"
/*"""
Classic cart-pole system implemented by Rich Sutton et al.
Copied from http://incompleteideas.net/sutton/book/code/pole.c
permalink: https://perma.cc/C9ZM-652R
"""
*/
// REFER TO https://github.com/openai/gym/blob/master/gym/envs/classic_control/cartpole.py
using Random = effolkronium::random_static;
using cairl::classic_control::renderer::Rectangle;
using cairl::classic_control::renderer::Circle;
using cairl::spaces::Discrete;
using cairl::spaces::Box;
using cairl::envs::Env;

namespace cairl::envs{

/*"""
Classic cart-pole system implemented by Rich Sutton et al.
Copied from http://incompleteideas.net/sutton/book/code/pole.c
permalink: https://perma.cc/C9ZM-652R
 // https://wr.perma-archives.org/public/c9zm-652r/im_/http://incompleteideas.net/sutton/book/code/pole.c
"""
*/
// REFER TO https://github.com/openai/gym/blob/master/gym/envs/classic_control/cartpole.py

    using ActionSpace = Discrete;
    class CartPoleEnv: public Env<
            ActionSpace,
            Box,
            double, 4, 1, 1>
    {
        //    Description:
        //        A pole is attached by an un-actuated joint to a cart, which moves along
        //        a frictionless track. The pendulum starts upright, and the goal is to
        //        prevent it from falling over by increasing and reducing the cart's
        //        velocity.
        //    Source:
        //        This environment corresponds to the version of the cart-pole problem
        //        described by Barto, Sutton, and Anderson
        //    Observation:
        //        Type: Box(4)
        //        Num     Observation               Min                     Max
        //        0       Cart Position             -4.8                    4.8
        //        1       Cart Velocity             -Inf                    Inf
        //        2       Pole Angle                -0.418 rad (-24 deg)    0.418 rad (24 deg)
        //        3       Pole Angular Velocity     -Inf                    Inf
        //    Actions:
        //        Type: Discrete(2)
        //        Num   Action
        //        0     Push cart to the left
        //        1     Push cart to the right
        //        Note: The amount the velocity that is reduced or increased is not
        //        fixed; it depends on the angle the pole is pointing. This is because
        //        the center of gravity of the pole increases the amount of energy needed
        //        to move the cart underneath it
        //    Reward:
        //        Reward is 1 for every step taken, including the termination step
        //    Starting State:
        //        All observations are assigned a uniform random value in [-0.05..0.05]
        //    Episode Termination:
        //        Pole Angle is more than 12 degrees.
        //        Cart Position is more than 2.4 (center of the cart reaches the edge of
        //        the display).
        //        Episode length is greater than 200.
        //        Solved Requirements:
        //        Considered solved when the average return is greater than or equal to
        //        195.0 over 100 consecutive trials.
        //    """

        enum Kinematics{
            EULER
        };

        enum Actions{
            LEFT,
            RIGHT,
            COUNT
        };

        // refer to https://github.com/openai/gym/blob/master/gym/envs/classic_control/cartpole.py for comments
        static constexpr double gravity = 9.8;
        static constexpr double masscart = 1.0;
        static constexpr double masspole = 0.1;
        static constexpr double total_mass = masspole + masscart;
        static constexpr double length = 0.5;
        static constexpr double polemass_length = masspole * length;
        static constexpr double force_mag = 10.0;
        static constexpr double tau = 0.02;
        static constexpr Kinematics kinematics_integrator = Kinematics::EULER;
        static constexpr double theta_threshold_radians = 12 * 2 * M_PI / 360;
        static constexpr double x_threshold = 2.4;
        static constexpr std::array<double, 4> high = {
                x_threshold * 2,
                std::numeric_limits<double>::max(),
                theta_threshold_radians * 2,
                std::numeric_limits<double>::max()
        };
        static constexpr std::array<double, 4> low = {
                -x_threshold * 2,
                std::numeric_limits<double>::min(),
                -theta_threshold_radians * 2,
                std::numeric_limits<double>::min()
        };

        int steps_beyond_done = 0;


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

        Rectangle cart = Rectangle(
                300,
                screen_height - carty - (cartheight / 2),
                cartwidth,
                cartheight,
                cairl::classic_control::renderer::Fill::FILLED,
                {0, 0, 0}
        );

        Rectangle pole = Rectangle(
                200,
                screen_height - polelen - carty,
                polewidth,
                polelen,
                cairl::classic_control::renderer::Fill::FILLED,
                {0.8, 0.6, 0.4}
        );

        // x / w, y / h
        Circle axle = Circle(
                200,
                screen_height  - carty - polewidth - 1,
                polewidth / 2,
                cairl::classic_control::renderer::Fill::FILLED,
                {.5, .5, .8}
        );

        const double CART_POS_X_MULTIPLIER = scale + screen_width;
        const double POLE_X_ADJUSTMENT_0 = (cart.data().cols / 2) - (pole.w / 2); // when rotation <= 0
        const double POLE_X_ADJUSTMENT_1 = (cart.data().cols / 2) + (pole.w / 2); // when rotation > 0
        const double AXLE_X_ADJUSTMENT = (cart.data().cols / 2) - (axle.w / 2);

        const Rectangle track = Rectangle(
                0,
                screen_height - carty,
                screen_width,
                1,
                cairl::classic_control::renderer::Fill::FILLED,
                {0, 0, 0}
        );

        cairl::classic_control::renderer::Renderer renderer{screen_width, screen_height};



    public:

        //        0       Cart Position             -4.8                    4.8
        //        1       Cart Velocity             -Inf                    Inf
        //        2       Pole Angle                -0.418 rad (-24 deg)    0.418 rad (24 deg)
        //        3       Pole Angular Velocity     -Inf                    Inf

        CartPoleEnv()
        : Env({2},{
                {-4.8, 4.8},
                {std::numeric_limits<double>::min(), std::numeric_limits<double>::max()},
                {-0.418, 0.418},
                {std::numeric_limits<double>::min(), std::numeric_limits<double>::max()}
        })
        {
            reset();
        }

        const cv::Mat& render(const char* = "human"/*mode*/) override{
            renderer.clear();

            // Set cart position
            cart.x = (state[0] *  scale + screen_width  - cartwidth) / 2.0;

            // Rotate pole
            pole.rotate(state[2]);

            // Set position of pole according to rotation
            if(pole.rot <= 0){
                pole.x = cart.x + POLE_X_ADJUSTMENT_0;
            }else if(pole.rot > 0){
                pole.x = cart.x  - (pole.data().cols) + POLE_X_ADJUSTMENT_1;
            }
            axle.x = cart.x + AXLE_X_ADJUSTMENT;

            // Render
            renderer.blit(track);
            renderer.blit(cart);
            renderer.blit(pole);
            renderer.blit(axle);
            return renderer.canvas;
        }



        StepReturnType step(ActionSpace as) override {
            auto action = as.n;

            if(as.contains(action)){
                throw std::runtime_error(fmt::format("{} ({}) invalid", action, "int"));
            }
            // 0 = x
            // 1 = x_dot
            // 2 = theta
            // 3 = theta_dot

            const auto force = (action == 1) ? force_mag : -force_mag;
            const auto costheta = std::cos(state[2]);
            const auto sintheta = std::sin(state[2]);

            const auto temp = (force + polemass_length * (state[3] * state[3] /*pow2*/) * sintheta) / total_mass;
            const auto thetaacc = (gravity * sintheta - costheta * temp) / (length * (4.0 / 3.0 - masspole * (costheta * costheta /*pow2*/)  / total_mass));

            const auto xacc = temp - polemass_length * thetaacc * costheta / total_mass;


            switch (kinematics_integrator) {
                case Kinematics::EULER:
                    state[0] = state[0] + tau * state[1];
                    state[1] = state[1] + tau * xacc;
                    state[2] = state[2] + tau * state[3];
                    state[3] = state[3] + tau * thetaacc;
                    break;
                default:
                    state[0] = state[1] + tau * xacc;
                    state[1] = state[0] + tau * state[1];
                    state[2] = state[3] + tau * thetaacc;
                    state[3] = state[2] + tau * state[3];
                    break;
            }
            terminal =
                    state[0] < -x_threshold ||
                    state[0] > x_threshold ||
                    state[2] < -theta_threshold_radians ||
                    state[2] > theta_threshold_radians;

            reward = 1.0;
            if(terminal){
                if(steps_beyond_done == 1){
                    SPDLOG_WARN("You are calling 'step()' even though this environment has already returned done = True. "
                                "You should always call 'reset()' once you receive 'done = True' "
                                "-- any further steps are undefined behavior.");

                }
                steps_beyond_done += 1;
                reward = 0.0;
            }

            return std::tie(state, reward, terminal, info);
        }


        /* Used to validate against openai GYM */
        void override_state(double x, double x_dot, double theta, double theta_dot){
            reset();
            state[0] = x;
            state[1] = x_dot;
            state[2] = theta;
            state[3] = theta_dot;
        }


        StateType& reset() override{
            state[0] = rng.get<std::uniform_real_distribution<>>(-0.05, 0.05);
            state[1] = rng.get<std::uniform_real_distribution<>>(-0.05, 0.05);
            state[2] = rng.get<std::uniform_real_distribution<>>(-0.05, 0.05);
            state[3] = rng.get<std::uniform_real_distribution<>>(-0.05, 0.05);
            steps_beyond_done = 0;
            terminal = false;
            reward = 0.0;
            info = {};
            return state;
        }





    };


} // namespace cairl::envs

#endif //CLASSIC_CONTROL_CARTPOLE_H
