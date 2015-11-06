#include "BasicTaskModeController.h"

BasicTaskModeController::BasicTaskModeController(const FlightMode::FlightModeStruct* flightMode)
{
    this->taskModeMappingInitialised = false;  
    this->flightMode = flightMode;
}

bool BasicTaskModeController::AssignTaskToMode (SchedulerTypes::Tasks task,FlightMode::Modes systemMode)
{
    bool addResult = false;
    
    if (task < SchedulerTypes::TotalAllowedTasks && this->taskModeMappingInitialised)
    { 
        this->taskModeMapping[task] = systemMode;
        addResult = true;
    }
    
    return addResult;
}
   
bool BasicTaskModeController::CanTaskExecute (SchedulerTypes::Tasks task)
{
    bool canExecute = false;
    
    if (task < SchedulerTypes::TotalAllowedTasks && this->taskModeMappingInitialised)
    {
        if (task == SchedulerTypes::ToggleHeartBeat ||
            task == SchedulerTypes::CheckCmdMsgReceived ||
            task == SchedulerTypes::SendSystemStatus ||
            task == SchedulerTypes::ReSendLastDataPck ||
            task == SchedulerTypes::CheckReceiveQueue ||
            task == SchedulerTypes::ProcessReceivedMsg ||
			task == SchedulerTypes::CommsSynch ||
			task == SchedulerTypes::HeartBeatMonitor)
        {
            canExecute = true; 
        }
        else
        {
            if (this->flightMode->mode == this->taskModeMapping[task])
            {
                canExecute = true;   
            }
        }
    }
    
    return canExecute;
}

void BasicTaskModeController::ResetTaskModeMapping()
{
    for (int iter = 0 ; iter < SchedulerTypes::TotalAllowedTasks; iter++)
    {   
        this->taskModeMapping[iter] = FlightMode::NotSet;   
    }
    
    this->taskModeMappingInitialised = true;
}
