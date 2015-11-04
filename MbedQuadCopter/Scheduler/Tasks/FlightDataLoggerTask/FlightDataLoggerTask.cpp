#include "FlightDataLoggerTask.h"
#include "mbed.h"

FlightDataLoggerTask::FlightDataLoggerTask(EncodersDecodersBase* encoder)
{
    this->encoder = encoder;      
}

void FlightDataLoggerTask::Init()
{
    
}
void FlightDataLoggerTask::ExecuteTask(int interval)
{
    if (this->encoder != NULL)
    {
        this->encoder->Encode();
    }    
}