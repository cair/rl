//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_SPACE_H
#define CLASSIC_CONTROL_SPACE_H
#include <any>

namespace cairl::spaces {
    class Space {

    public:

        [[nodiscard]] virtual bool contains(const std::any& x) const = 0;


    };
}

#endif //CLASSIC_CONTROL_SPACE_H
