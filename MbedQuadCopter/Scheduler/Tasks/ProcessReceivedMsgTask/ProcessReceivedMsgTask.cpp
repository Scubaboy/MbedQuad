#include "ProcessReceivedMsgTask.h"
#include "mbed.h"

ProcessReceivedMsgTask::ProcessReceivedMsgTask(BaseComms* comms)
{
    this->comms = comms;   
}

void ProcessReceivedMsgTask::ExecuteTask(int interval)
{
    if (comms != NULL)
    { 
        this->comms->DataReceived();  
        
    }  
}

void ProcessReceivedMsgTask::Init()
{
    
}