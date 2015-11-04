#include "SystemStatusLoggerTask.h"
#include "mbed.h"

SystemStatusLoggerTask::SystemStatusLoggerTask(EncodersDecodersBase* encoder)
{
    this->encoder = encoder;      
}

void SystemStatusLoggerTask::Init()
{
    
}

void SystemStatusLoggerTask::ExecuteTask(int interval)
{
    if (this->encoder != NULL)
    {
        this->encoder->Encode();
    }    
}