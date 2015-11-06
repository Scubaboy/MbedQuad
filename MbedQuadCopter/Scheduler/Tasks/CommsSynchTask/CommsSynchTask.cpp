#include "CommsSynchTask.h"
#include "mbed.h"

CommsSynchTask::CommsSynchTask(BaseComms* sysComms, SystemModeControllerBase* systemModeController)
{
    this->sysComms = sysComms;
    this->systemModeController = systemModeController;     
    this->waitingSynch = true; 
    this->mode = FlightMode::StartUp;
}


void CommsSynchTask::ExecuteTask(int interval)
{
    if (this->sysComms != NULL && this->systemModeController != NULL)
    {
       if (!this->sysComms->isSynched())// && this->waitingSynch)
        {
    //        this->systemModeController->SetMode(this->mode);
    //        this->waitingSynch = false;
    //    }
    //    else if (this->waitingSynch == false && this->sysComms->isSynched())
    //    {
    //        this->mode = this->systemModeController->GetPreviousMode();
    //        this->sysComms->ReSynch();
    //        this->waitingSynch = true;
    //    }
     //   else
    //    {
            this->sysComms->SendSynch();   
        }
    }  
}

void CommsSynchTask::Init()
{
    
} 
