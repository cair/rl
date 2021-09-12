//
// Created by per on 8/4/21.
//

#ifndef CLASSIC_CONTROL_MULTITASK_H
#define CLASSIC_CONTROL_MULTITASK_H
#include "cairl/contrib/environments/flashrl/FlashEnv.h"

namespace cairl::envs::flashrl{

    class MultitaskEnv: public FlashEnv{
    public:
        MultitaskEnv(): FlashEnv("Multitask-v2", "https://flashrl.sysx.no/506546_MultitaskMOCHIcomplete202c.swf"){

        };
    };

} // namespace cairl::envs::flashrl



#endif //CLASSIC_CONTROL_MULTITASK_H
