#include "HighLevelFlightControllerTask.h"
#include "mbed.h"

const float MilliSecondsInSecond = 1000.0f;
const float AllowTimeDelta = 0.020f;

HighLevelFlightControllerTask::HighLevelFlightControllerTask(BaseHighLevelRateController* HighLevelController)
{
    this->HighLevelController = HighLevelController;
    this->initRun = false;
}

void HighLevelFlightControllerTask::ExecuteTask(int interval)
{
    if (!this->initRun)
    {
        this->Init();
    }
    
    float timeDelta = (float)interval/(float)MilliSecondsInSecond;
        
    if (timeDelta <= AllowTimeDelta)
    {
           this->HighLevelController->Run();    
    }
}

void HighLevelFlightControllerTask::Init()
{
    if (this->HighLevelController != NULL)
    {
        
        this->initRun = true;  
    }
}