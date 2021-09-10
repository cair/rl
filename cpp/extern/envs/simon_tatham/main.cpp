//
// Created by per on 8/13/21.
//
#include "simon_tatham/envs/LightUp.h"

int main(){

    auto instance = LightUp(7, 7, LightUp::HARD, LightUp::SYMMETRY_ROT2, 20);
//    instance.generate_solution_action_set();
//    instance.generate_screenshot();
//    instance.generate_screenshot(true);
//    instance.generate_board();
//    instance.solve();
//    instance.generate_board();
//
//    exit(1);
//
    LightUp::generate_dataset(
            {4, 21},
            {20, 40, 60},
            {
                LightUp::Difficulty::EASY,
            },
            {
                LightUp::Symmetry::SYMMETRY_NONE,
            },
            true
    );

    LightUp::generate_dataset(

            {30, 35},
            {20, 40, 60},
            {
                LightUp::Difficulty::EASY,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );

    LightUp::generate_dataset(

            {40, 45},
            {20, 40, 60},
            {
                LightUp::Difficulty::EASY,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );

    LightUp::generate_dataset(

            {4, 21},
            {20, 40, 60},
            {
                LightUp::Difficulty::TRICKY,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );


    LightUp::generate_dataset(

            {30, 35},
            {20, 40, 60},
            {
                LightUp::Difficulty::TRICKY,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );

    LightUp::generate_dataset(

            {40, 45},
            {20, 40, 60},
            {
                LightUp::Difficulty::TRICKY,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );

    LightUp::generate_dataset(

            {4, 21},
            {20, 40, 60},
            {
                LightUp::Difficulty::HARD,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );

    LightUp::generate_dataset(

            {30, 35},
            {20, 40, 60},
            {
                LightUp::Difficulty::HARD,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );

    LightUp::generate_dataset(

            {40, 45},
            {20, 40, 60},
            {
                LightUp::Difficulty::HARD,
                },
                {
                LightUp::Symmetry::SYMMETRY_NONE,
                },
                true
                );

}