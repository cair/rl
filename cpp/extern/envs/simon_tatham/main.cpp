//
// Created by per on 8/13/21.
//
#include "simon_tatham/envs/LightUp.h"

int main(){

    auto instance = LightUp(7, LightUp::HARD, LightUp::SYMMETRY_ROT2, 20);
//    instance.generate_solution_action_set();
//    instance.generate_screenshot();
//    instance.generate_screenshot(true);
//    instance.generate_board();
//    instance.solve();
//    instance.generate_board();
//
//    exit(1);

    auto datasets = LightUp::generate_dataset(
            {2, 22},
            {20, 30, 40, 50, 60, 70, 80},
            {
                LightUp::Difficulty::EASY,
                LightUp::Difficulty::HARD,
                LightUp::Difficulty::TRICKY
            },
            {
                LightUp::Symmetry::SYMMETRY_NONE,
                LightUp::Symmetry::SYMMETRY_REF2,
                LightUp::Symmetry::SYMMETRY_REF4,
                LightUp::Symmetry::SYMMETRY_ROT2,
                LightUp::Symmetry::SYMMETRY_ROT4
            },
            true
    );

}