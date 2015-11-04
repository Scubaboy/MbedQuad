#include "CheckCommsReceiveQueueTask.h"
#include "mbed.h"

CheckCommsReceiveQueueTask::CheckCommsReceiveQueueTask(BaseComms* comms)
{
    this->comms = comms;   
}

void CheckCommsReceiveQueueTask::ExecuteTask(int interval)
{
    if (comms != NULL)
    {       
        this->comms->DataReceived();        
    }  
}

void CheckCommsReceiveQueueTask::Init()
{
    
}