#include "GPSTask.h"
#include "mbed.h"

GPSTask::GPSTask(GPSBase* gps)
{
    this->gps = gps;  
    this->initRun = false; 
}


void GPSTask::ExecuteTask(int interval)
{
    if (this->gps != NULL)
    {
        if (!this->initRun)
        {
            this->Init();
        }
        else
        {
            this->gps->ReadPositionData();
        }
    }
}

void GPSTask::Init()
{
    this->initRun = this->gps->Initialise();
}