#include "LowLevelFlightControllerTask.h"
#include "mbed.h"

const float MilliSecondsInSecond = 1000.0f;
const float AllowTimeDelta = 0.010f;

LowLevelFlightControllerTask::LowLevelFlightControllerTask(BasicLowLevelRateController* lowLevelController)
{
    this->lowLevelController = lowLevelController;
    this->initRun = false;
}

void LowLevelFlightControllerTask::ExecuteTask(int interval)
{
    if (!this->initRun)
    {
        this->Init();
    }
    
    float timeDelta = (float)interval/(float)MilliSecondsInSecond;
        
    if (timeDelta <= AllowTimeDelta)
    {
        this->lowLevelController->RateControllerRun(timeDelta);        
    }
}

void LowLevelFlightControllerTask::Init()
{
    if (this->lowLevelController != NULL)
    {
        this->lowLevelController->InitTargets();
        this->lowLevelController->Init(); 
        this->initRun = true;  
    }
}

