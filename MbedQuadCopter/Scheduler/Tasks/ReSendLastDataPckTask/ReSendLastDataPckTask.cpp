#include "ReSendLastDataPckTask.h"
#include "mbed.h"

ReSendLastDataPckTask::ReSendLastDataPckTask(BaseComms* comms)
{
    this->comms = comms;   
}

void ReSendLastDataPckTask::ExecuteTask(int interval)
{
    if (comms != NULL)
    { 
        this->comms->ReSendLastMsg();  
        
    }  
}

void ReSendLastDataPckTask::Init()
{
    
}