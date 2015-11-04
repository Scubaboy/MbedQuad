#include "ConfigureCalibrateTask.h"
#include "mbed.h"

ConfigureCalibrateTask::ConfigureCalibrateTask(BaseSysCalibrationController** calController, BaseSysConfigController** configController, DebugLoggerBase* debugLogger)
{
    this->configureCalibrateComplete = false;    
    this->calController = calController; 
    this->configController = configController;
    this->calComplete = false;
    this->configComplete = false;
    this->debugLogger = debugLogger;
}


void ConfigureCalibrateTask::ExecuteTask(int interval)
{
    if (!this->configureCalibrateComplete)
    {
        //Start calibration
        if (*this->calController != NULL && !this->calComplete)
        {
            if (this->debugLogger != NULL)
            {
                this->debugLogger->Log("Running calibration controller\n\r\0");   
            }
            
            this->calComplete = (*this->calController)->CalibrateSystem();
        }
        else if (*this->configController != NULL && !this->configComplete)
        {
            if (this->debugLogger != NULL)
            {
                this->debugLogger->Log("Running configuration controller\n\r\0");   
            }
            
            this->configComplete = (*this->configController)->Configure();
        }
        else
        {
            ConfigCalCompleteDataPck::ConfigCalComplete configCalComplete;
                
            this->configureCalibrateComplete = true;
            configCalComplete.type = DataPckTypes::ConfigCalComplete;
            configCalComplete.complete = true;
            this->SendRequest(&configCalComplete);
        } 
    }
}


void ConfigureCalibrateTask::Init()
{
    
}

void ConfigureCalibrateTask::SendRequest(ConfigCalCompleteDataPck::ConfigCalComplete* configCalComplete)
{
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
        if (!(this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)configCalComplete, sizeof(ConfigCalCompleteDataPck::ConfigCalComplete),true))
        {
            
        }
    }
}