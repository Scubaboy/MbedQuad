#ifndef BaseHighLevelCtrlAction_H
#define BaseHighLevelCtrlAction_H
#include "BaseResponse.h"

class BaseHighLevelCtrlAction
{
    public:
        virtual bool Compteleted() = 0;
        virtual BaseResponse::BaseResp* Run() = 0; 
};
#endif