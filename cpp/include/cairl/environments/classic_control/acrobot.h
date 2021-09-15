//
// Created by per on 9/10/21.
//

#ifndef CLASSIC_CONTROL_ACROBOT_H
#define CLASSIC_CONTROL_ACROBOT_H
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


// REFER TO https://github.com/openai/gym/blob/master/gym/envs/classic_control/acrobot.py
    using cairl::spaces::Space;



//        Acrobot is a 2-link pendulum with only the second joint actuated.
//        Initially, both links point downwards. The goal is to swing the
//        end-effector at a height at least the length of one link above the base.
//        Both links can swing freely and can pass by each other, i.e., they don't
//        collide when they have the same angle.
//        **STATE:**
//                The state consists of the sin() and cos() of the two rotational joint
//                angles and the joint angular velocities :
//        [cos(theta1) sin(theta1) cos(theta2) sin(theta2) thetaDot1 thetaDot2].
//        For the first link, an angle of 0 corresponds to the link pointing downwards.
//        The angle of the second link is relative to the angle of the first link.
//        An angle of 0 corresponds to having the same angle between the two links.
//        A state of [1, 0, 1, 0, ..., ...] means that both links point downwards.
//        **ACTIONS:**
//                The action is either applying +1, 0 or -1 torque on the joint between
//                the two pendulum links.
//        .. note::
//        The dynamics equations were missing some terms in the NIPS paper which
//        are present in the book. R. Sutton confirmed in personal correspondence
//                that the experimental results shown in the paper and the book were
//        generated with the equations shown in the book.
//        However, there is the option to run the domain with the paper equations
//        by setting book_or_nips = 'nips'
//                                  **REFERENCE:**
//        .. seealso::
//        R. Sutton: Generalization in Reinforcement Learning:
//                Successful Examples Using Sparse Coarse Coding (NIPS 1996)
//        .. seealso::
//        R. Sutton and A. G. Barto:
//                Reinforcement learning: An introduction.
//        Cambridge: MIT press, 1998.
//        .. warning::
//        This version of the domain uses the Runge-Kutta method for integrating
//                the system dynamics and is more realistic, but also considerably harder
//        than the original version which employs Euler integration,
//                see the AcrobotLegacy class.
    class AcrobotEnv: public Env<Discrete, Box>
    {

        static constexpr double LINK_LENGTH_1 = 1.0;  // # [m]
        static constexpr double LINK_LENGTH_2 = 1.0;  //  # [m]
        static constexpr double LINK_MASS_1 = 1.0;  //  #: [kg] mass of link 1
        static constexpr double LINK_MASS_2 = 1.0;  //  #: [kg] mass of link 2
        static constexpr double LINK_COM_POS_1 = 0.5;  //  #: [m] position of the center of mass of link 1
        static constexpr double LINK_COM_POS_2 = 0.5;  //  #: [m] position of the center of mass of link 2
        static constexpr double LINK_MOI = 1.0;  //  #: moments of inertia for both links

        static constexpr double MAX_VEL_1 = 4 * M_PI;
        static constexpr double MAX_VEL_2 = 9 * M_PI;

        static constexpr std::array<double, 3> AVAIL_TORQUE = {-1.0, 0.0, +1};



        //#: use dynamics equations from the nips paper or the book
        enum DynamicEquation{
            BOOK,
            NIPS
        };

        DynamicEquation dynamic_equation = BOOK;

        static constexpr int actions_num = 3;

//        action_arrow = None
//        domain_fig = None
//        actions_num = 3

        /// Member properties
        double torque_noise_max = 0.0;
        double dt = 0.2;
        StateType state2;



    public:


        AcrobotEnv()
            : Env(
                "Acrobot-v3",
                {},
                {actions_num},
                {{6, 1, 1}}
        ), state2(state)
        {
            reset();
        }

        const cv::Mat& render(const char* = "human"/*mode*/) override{
           return dummy;
        }




        StateType _dsdt(const StateType& s, double t, double a) {
            const auto m1 = LINK_MASS_1;
            const auto m2 = LINK_MASS_2;
            const auto l1 = LINK_LENGTH_1;
            const auto lc1 = LINK_COM_POS_1;
            const auto lc2 = LINK_COM_POS_2;
            const auto I1 = LINK_MOI;
            const auto I2 = LINK_MOI;
            const auto g = 9.8;
            const auto theta1 = s[0];
            const auto theta2 = s[1];
            const auto dtheta1 = s[2];
            const auto dtheta2  = s[3];
            const auto d1 = (
                    m1 * std::pow(lc1, 2)
                    + m2 * (std::pow(l1, 2) + std::pow(lc2, 2) + 2 * l1 * lc2 * std::cos(theta2))
                    + I1
                    + I2
            );
            const auto d2 = m2 * (std::pow(lc2, 2) + l1 * lc2 * std::cos(theta2)) + I2;
            const auto phi2 = m2 * lc2 * g * std::cos(theta1 + theta2 - M_PI / 2.0);
            const auto phi1 = (
                    -m2 * l1 * lc2 * std::pow(dtheta2, 2) * std::sin(theta2)
                    - 2 * m2 * l1 * lc2 * dtheta2 * dtheta1 * std::sin(theta2)
                    + (m1 * lc1 + m2 * l1) * g * std::cos(theta1 - M_PI / 2)
                    + phi2
            );
            double ddtheta2;
            if(dynamic_equation == DynamicEquation::NIPS){
                //        # the following line is consistent with the description in the
                //        # paper
                ddtheta2 = (a + d2 / d1 * phi1 - phi2) / (m2 * std::pow(lc2, 2) + I2 - std::pow(d2, 2) / d1);
            }else{
                //# the following line is consistent with the java implementation and the
                //# book
                ddtheta2 = (a + d2 / d1 * phi1 - m2 * l1 * lc2 * std::pow(dtheta1, 2) * std::sin(theta2) - phi2
                           ) / (m2 * std::pow(lc2, 2) + I2 - std::pow(d2, 2) / d1);
            }
            const auto ddtheta1 = -(d2 * ddtheta2 + phi1) / d1;

            return {dtheta1, dtheta2, ddtheta1, ddtheta2};
        }


        static double wrap(double x, double m, double M){
            const auto diff = M - m;
            while(x > M) x = x - diff;
            while(x < m) x = x + diff;
            return x;
        }

        static double bound(double x, double m, double M){
            return std::min(std::max(x, m), M);
        }

        StateType rk4(const StateType& s, double torque, double t0, double t1) {

            auto thist = t0;
            auto _dt = t1 - thist;
            auto dt2 = _dt / 2.0;
            auto y0 = s;
            auto k1 = _dsdt(y0, thist, torque);
            auto k2 = _dsdt(y0 + dt2 * k1, thist + dt2, torque);
            auto k3 = _dsdt(y0 + dt2 * k2, thist + dt2, torque);
            auto k4 = _dsdt(y0 + _dt * k3, thist + _dt, torque);
            return y0 + _dt / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);
        }

        StepReturnType step(ActionType as) override {
            auto action = as;

            if(!action_space.contains(action)){
                throw std::runtime_error(fmt::format("{} ({}) invalid", action, "int"));
            }

            auto torque = AVAIL_TORQUE[action];

            if(torque_noise_max > 0){
                torque += Random::get<double>(-torque_noise_max, torque_noise_max);
            }

            auto ns = rk4(state, torque, 0, dt);
            state[0] = wrap(ns[0], -M_PI, M_PI);
            state[1] = wrap(ns[1], -M_PI, M_PI);
            state[2] = bound(ns[2], -MAX_VEL_1, MAX_VEL_2);
            state[3] = bound(ns[3], -MAX_VEL_1, MAX_VEL_2);
            terminal = -std::cos(state[0]) - std::cos(state[1] + state[0]) > 1.0;
            reward = (terminal) ? -1.0 : 0.0;

            state2[0] = std::cos(state[0]);
            state2[1] = std::sin(state[0]);
            state2[2] = std::cos(state[1]);
            state2[3] = std::sin(state[1]);
            state2[4] = state[2];
            state2[5] = state[3];

            return std::tie(state2, reward, terminal, info);
        }


        /* Used to validate against openai GYM */
        void override_state(double a, double b, double c, double d){
            reset();
            state[0] = a;
            state[1] = b;
            state[2] = c;
            state[3] = d;
            state[5] = 0;
            state[6] = 0;
        }


        StateType& reset() override{
            state[0] = rng.get<std::uniform_real_distribution<>>(-0.1, 0.1);
            state[1] = rng.get<std::uniform_real_distribution<>>(-0.1, 0.1);
            state[2] = rng.get<std::uniform_real_distribution<>>(-0.1, 0.1);
            state[3] = rng.get<std::uniform_real_distribution<>>(-0.1, 0.1);
            state[5] = 0;
            state[6] = 0;
            terminal = false;
            reward = 0.0;
            info = {};
            return state;
        }





    };


} // namespace cairl::envs



#endif //CLASSIC_CONTROL_ACROBOT_H
