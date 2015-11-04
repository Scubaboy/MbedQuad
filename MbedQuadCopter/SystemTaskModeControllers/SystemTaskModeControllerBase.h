#ifndef SystemTaskModeControllerBase_H
#define SystemTaskModeControllerBase_H
#include "SchedulerTypes.h"
#include "FlightMode.h"

class SystemTaskModeControllerBase
{
    public:
        virtual bool AssignTaskToMode (SchedulerTypes::Tasks task,FlightMode::Modes systemMode) = 0;   
        virtual bool CanTaskExecute (SchedulerTypes::Tasks task) = 0;
        virtual void ResetTaskModeMapping() = 0;
};
#endif