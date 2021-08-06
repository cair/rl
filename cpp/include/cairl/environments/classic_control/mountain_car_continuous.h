//
// Created by per on 7/28/21.
//

#ifndef CLASSIC_CONTROL_MOUNTAIN_CAR_CONTINUOUS_H
#define CLASSIC_CONTROL_MOUNTAIN_CAR_CONTINUOUS_H
#include "cairl/environments/Env.h"
#include "cairl/spaces/Discrete.h"


using ActionSpace = cairl::spaces::Discrete;

namespace cairl::envs{




    class ContinuousMountainCarEnv: public Env<
            ActionSpace,
            Box,
            double, 4, 1, 1>{
        // """
        //@author: Olivier Sigaud
        //A merge between two sources:
        //* Adaptation of the MountainCar Environment from the "FAReinforcement" library
        //of Jose Antonio Martin H. (version 1.0), adapted by  'Tom Schaul, tom@idsia.ch'
        //and then modified by Arnaud de Broissia
        //* the OpenAI/gym MountainCar environment
        //itself from
        //http://incompleteideas.net/sutton/MountainCar/MountainCar1.cp
        //permalink: https://perma.cc/6Z2N-PFWC
        //"""

        // Mountain Car Global variables:
        float mcar_position, mcar_velocity;

        /*constexpr double mcar_min_position -1.2;
        constexpr double mcar_max_position 0.6;
        constexpr double mcar_max_velocity 0.07;            // the negative of this is also the minimum velocity
        constexpr double mcar_goal_position 0.5;
        constexpr double POS_WIDTH = (1.7 / 8);               // the tile width for position
        constexpr double VEL_WIDTH = (0.14 / 8);              // the tile width for velocity*/


        /*
         *
        void load_F()
        // Compute feature sets for current car state
           {float state_vars[2];
            state_vars[0] = mcar_position / POS_WIDTH;
            state_vars[1] = mcar_velocity / VEL_WIDTH;
            for (int a=0; a<M; a++)
                GetTiles(&F[a][0],NUM_TILINGS,state_vars,2,N,a);}

        void mcar_init()
        // Initialize state of Car
           {mcar_position = -0.5;
            mcar_velocity = 0.0;}

        void mcar_step(int a)
        // Take action a, update state of car
           {mcar_velocity += (a-1)*0.001 + cos(3*mcar_position)*(-0.0025);
            if (mcar_velocity > mcar_max_velocity) mcar_velocity = mcar_max_velocity;
            if (mcar_velocity < -mcar_max_velocity) mcar_velocity = -mcar_max_velocity;
            mcar_position += mcar_velocity;
            if (mcar_position > mcar_max_position) mcar_position = mcar_max_position;
            if (mcar_position < mcar_min_position) mcar_position = mcar_min_position;
            if (mcar_position==mcar_min_position && mcar_velocity<0) mcar_velocity = 0;}

        bool mcar_goal_p ()
        // Is Car within goal region?
           {return mcar_position >= mcar_goal_position;}


         */

    };


}

#endif //CLASSIC_CONTROL_MOUNTAIN_CAR_CONTINUOUS_H
