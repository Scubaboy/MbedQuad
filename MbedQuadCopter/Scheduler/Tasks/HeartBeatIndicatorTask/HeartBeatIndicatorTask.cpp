#include "HeartBeatIndicatorTask.h"
#include "mbed.h"

HeartBeatIndicatorTask::HeartBeatIndicatorTask(HeartBeatIndicator* heartBeat)
{
    this->heartBeat = heartBeat;    
}

void HeartBeatIndicatorTask::Init()
{
    
}

void HeartBeatIndicatorTask::ExecuteTask(int interval)
{
    if (this->heartBeat != NULL)
    {
        this->heartBeat->UpdateHeartBeat();
    }
}