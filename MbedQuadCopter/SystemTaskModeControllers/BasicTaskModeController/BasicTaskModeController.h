#ifndef BasicModeController_H
#define BasicModeController_H
#include "SystemTaskModeControllerBase.h"

class BasicTaskModeController : public SystemTaskModeControllerBase
{
    public:
        BasicTaskModeController(const FlightMode::FlightModeStruct* flightMode);
        virtual bool AssignTaskToMode (SchedulerTypes::Tasks task,FlightMode::Modes systemMode);   
        virtual bool CanTaskExecute (SchedulerTypes::Tasks task);  
        virtual void ResetTaskModeMapping();
    private:
        FlightMode::Modes taskModeMapping[SchedulerTypes::TotalAllowedTasks];
        bool taskModeMappingInitialised;
        const FlightMode::FlightModeStruct* flightMode;
};
#endif