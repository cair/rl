//
// Created by per on 8/15/21.
//

#ifndef CAIRL_SIMON_TATHAM_PUZZLE_TIMELIMIT_H
#define CAIRL_SIMON_TATHAM_PUZZLE_TIMELIMIT_H
#include "cairl/environments/Env.h"

namespace cairl::wrappers {
    using cairl::envs::Env;

    template<int limit, class Environment>
    class TimeLimit: public Environment{
        int counter = 0;
        std::shared_ptr<typename Environment::StepReturnType> last_return = nullptr;

    public:

        typename Environment::StepReturnType step(ActionSpace a) override{
            counter++;
            if(counter > limit && last_return != nullptr){
                return *last_return;
            }

            auto tuple =  Environment::step(a);

            if(counter >= limit){
                std::get<2>(tuple) = true;
                last_return = std::make_shared<typename Environment::StepReturnType>(tuple);
            }

            return tuple;
        }

        typename Environment::StateType& reset() override{
            counter = 0;
            last_return = nullptr;
            return Environment::reset();
        }

    };


} // namespace cairl::wrappers

#endif //CAIRL_SIMON_TATHAM_PUZZLE_TIMELIMIT_H
