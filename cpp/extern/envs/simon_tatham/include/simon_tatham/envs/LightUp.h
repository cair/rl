//
// Created by per on 8/13/21.
//

#ifndef CAIRL_SIMON_TATHAM_PUZZLE_LIGHTUP_H
#define CAIRL_SIMON_TATHAM_PUZZLE_LIGHTUP_H


#include <string>
#include <iostream>
#include <random>
#include <limits>
#include <omp.h>
#include <mutex>
#include <filesystem>
#include <fstream>
#include "StringUtils.h"

extern "C"{
    #include <custom_impl.c>
    #include <drawing.c>
    #include <lightup.c>
    #include "../gtk.c"

};
class LightUp{
public:
    enum Difficulty{
        EASY = 0,
        TRICKY = 1,
        HARD = 2,
    };

    enum Symmetry{
        SYMMETRY_NONE,
        SYMMETRY_REF2,
        SYMMETRY_ROT2,
        SYMMETRY_REF4,
        SYMMETRY_ROT4,
        SYMMETRY_MAX
    };
private:
    // Seed with a real random value, if available
    std::random_device r;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1;
    std::uniform_int_distribution<int64_t> uniform_dist;
    const std::string game_name = "Lights Up";



    game_state *state;
    game_params* params;
    std::string seed;
    std::string desc;
    std::string game_id;
    Difficulty difficulty;
    Symmetry symmetry;
    int black_piece_age;

public:
    static const int DEFAULT_BLACK_PIECE_AGE = 20;

    explicit LightUp(
            int size,
            Difficulty difficulty,
            Symmetry symmetry = SYMMETRY_NONE,
            int black_piece_age = DEFAULT_BLACK_PIECE_AGE
    )
    : e1(r())
    , difficulty(difficulty)
    , symmetry(symmetry)
    , black_piece_age(black_piece_age)
    , uniform_dist(
            647440323558759,
            1647440323558759)
    , seed(std::to_string(uniform_dist(e1)))
    , params(default_params()){
        /// Set parameters
        params->w = size;
        params->h = size;
        params->difficulty = difficulty;
        params->blackpc = black_piece_age;  // Age of black pieces
        params->symm = symmetry;

        /// Seed the random state of the game
        random_state* rs = random_new(seed.c_str(), 32);

        /// Generate a game description
        desc = new_game_desc(params , rs, NULL , false);
        game_id = std::to_string(params->w) + "x" + std::to_string(params->h) + ":" + desc;

        /// validate that the description is valid
        auto err = validate_desc(params, desc.data());
        if (err) {
            std::cout << "Board description is invalid for LightUp" << std::endl;
            return;
        }

        /// Generate a new game state
        state = new_game(NULL, params, desc.data());

        /// Cleanup
        delete err;
    }



    std::string get_solve_action_set(){
        /// Generate a action/move string that leads to solved state
        if(state->completed){
            std::cout << "Cannot generate solution when the game is solved already!" << std::endl;
        }
        const char *msg = NULL;
        std::string move_str = solve_game(state, state, NULL, &msg);

        assert(!move_str.empty() && !msg);  // The move string is not valid or there was an error
        delete msg;
        return move_str;
    }

    bool solve(){
        /// Apply actions on on the game state
        game_state *new_state;
        new_state = execute_move(state, get_solve_action_set().c_str());
        free_game(state);
        state = new_state;
    }

    std::string get_id(){
        return game_id;
    }


    void generate_solution_action_set(){
        if(!std::filesystem::exists("output")){
            std::filesystem::create_directory("output");
        }
        auto solution = get_solve_action_set();
        auto file_name = generate_generic_file_name(true) + "_solution.txt";

        /// Write to file
        std::ofstream file(file_name);
        file.write(solution.data(), solution.size());
        file.close();


    }

    void generate_screenshot(bool solve = false){
        if(!std::filesystem::exists("output")){
            std::filesystem::create_directory("output");
        }

        int argc = 0;
        char **argv = NULL;
        std::string arg_cpp = get_id();


        auto screenshot_file = generate_generic_file_name(solve) + ".png";

        bool headless = TRUE;
        int argtype = ARG_ID;
        frontend *fe;
        char *error;

        /// Screenshot?
        static int s_id {0};
        if(s_id++ == 0){
            gtk_init(&argc, &argv);
        }

        fe = new_window(arg_cpp.data(), argtype, &error, headless);

        if (!fe) {
            fprintf(stderr, "%s: %s\n", game_name.data(), error);
            exit(0);
        }

        if(solve){
            midend_solve(fe->me);
        }

        midend_force_redraw(fe->me);
        save_screenshot_png(fe, screenshot_file.data());


        midend_free(fe->me);
        delete fe;
    }

    std::string generate_generic_file_name(bool solved){
        return string_format(
                "output/%s_%s_%s---%s_%s",
                std::to_string(difficulty).c_str(),
                std::to_string(symmetry).c_str(),
                std::to_string(black_piece_age).c_str(),
                get_id().c_str(),
                (solved) ? "solved" : "unsolved"
        );
    }

    void generate_board(){
        auto board = get_board();

        auto board_file_name = generate_generic_file_name(state->completed) + ".txt";



        /// Write to file
        std::ofstream file(board_file_name);
        file.write(board.data(), board.size());
        file.close();
    }


    static std::vector<std::string> generate_dataset(
            std::pair<int, int> min_max,
            const std::vector<int>& black_percentages,
            const std::vector<Difficulty>& difficulties,
            const std::vector<Symmetry>& symmetries,
            bool screenshot
    ){
        if(!std::filesystem::exists("output")){
            std::filesystem::create_directory("output");
        }

        if(min_max.first <= 0){
            std::cerr << "Min-Size must be larger than 0!" << std::endl;
            throw std::runtime_error("Min-Size must be larger than 0!");
        }

        std::mutex g_lock;
        std::vector<std::string> results;

        #pragma omp parallel  for
        for(int x = min_max.first; x < min_max.second; x++){
            for(auto difficulty : difficulties){
                for(auto symmetry : symmetries){
                    for(auto b: black_percentages){
                        auto instance = LightUp(x, difficulty, static_cast<Symmetry>(symmetry), b);

                        {
                            std::lock_guard stahp(g_lock);


                            /// Generate solution string
                            instance.generate_solution_action_set();

                            /// Generate unsolved version
                            if(screenshot){
                                instance.generate_screenshot();
                            }
                            instance.generate_board();

                            instance.solve();

                            /// Generate solved version
                            if(screenshot){
                                instance.generate_screenshot(true);
                            }
                            instance.generate_board();




                            std::cout << results.size() << "(" << x << ", " << x << ")" << std::endl;
                        }

                    }
                }
            }
        }


    }


    std::string get_board(){
        char c = '?';
        std::string map;
        for (int y = 0; y < state->h; y++) {
            for (int x = 0; x < state->w; x++) {
                c = '.';
                if (GRID(state, flags, x, y) & F_BLACK) {
                    if (GRID(state, flags, x, y) & F_NUMBERED)
                        c = GRID(state, lights, x, y) + '0';
                    else
                        c = '#';
                } else {
                    if (GRID(state, flags, x, y) & F_LIGHT)
                        c = 'O';
                    else if (GRID(state, flags, x, y) & F_IMPOSSIBLE)
                        c = 'X';
                }
                map += c;
            }
            map += "\n";
        }

        return map;
    }

    ~LightUp(){
        free_game(state);
    }
};

#endif //CAIRL_SIMON_TATHAM_PUZZLE_LIGHTUP_H
