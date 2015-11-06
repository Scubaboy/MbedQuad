#include "ModeToTaskSetup.h"
#include "mbed.h"

void ModeToTaskSetup::SetupModeTaskMapping(SystemTaskModeControllerBase* modeController)
{
    if (modeController != NULL)
    {
      //  modeController->AssignTaskToMode(SchedulerTypes::CommsSynch,FlightMode::CommsSynch);
        modeController->AssignTaskToMode(SchedulerTypes::StartUp,FlightMode::StartUp);
        modeController->AssignTaskToMode(SchedulerTypes::PWMTest,FlightMode::PWMRangeTest);
        modeController->AssignTaskToMode(SchedulerTypes::ConfigCal,FlightMode::ConfigCal);
        modeController->AssignTaskToMode(SchedulerTypes::IMUUpdate, FlightMode::Active);
        modeController->AssignTaskToMode(SchedulerTypes::MPUUpdate, FlightMode::Active);
        modeController->AssignTaskToMode(SchedulerTypes::UpdateLowLevelRateCtrl, FlightMode::Active);
        modeController->AssignTaskToMode(SchedulerTypes::UpdateHighLevelRateCtrl, FlightMode::Active);
        modeController->AssignTaskToMode(SchedulerTypes::SendFlightData, FlightMode::Active);
        modeController->AssignTaskToMode(SchedulerTypes::FlightPackMonitor, FlightMode::Active);
        modeController->AssignTaskToMode(SchedulerTypes::AltimeterUpdate, FlightMode::Active);
        modeController->AssignTaskToMode(SchedulerTypes::GPSUpdate, FlightMode::Active);
        
    } 
}
