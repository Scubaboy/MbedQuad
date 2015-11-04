#ifndef BaseHighLevelRateController_H
#define BaseHighLevelRateController_H
#include "BaseHighLevelCtrlAction.h"
#include "HighLevelCtrlActionTypes.h"

class BaseHighLevelRateController
{
    public:
        virtual void Init() = 0; 
        virtual void Run() =0;
        virtual bool AddAction(HighLevelCtrlActionTypes::ActionRequest actionRequest) = 0; 
};
#endif