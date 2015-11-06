#include "HeartBeatMonitor.h"
#include "mbed.h"
#include "DataPckTypes.h"


HeartBeatMonitor::HeartBeatMonitor(HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus, CommsStatus::CommsStatusStruct* commsStatus, DebugLoggerBase* debugLogger)
{
     this->heartBeatStatus = heartBeatStatus;
     this->commsStatus = commsStatus;
     this->debugLogger = debugLogger;
}

void HeartBeatMonitor::SendHeartBeat()
{
    if (this->commsStatus != NULL && this->commsStatus->mode != CommTypes::Synching)
    {
        this->SendToLogger("Heartbeat sent\n\r\0");
        HeartBeatDataPck::HeartBeatData heartBeat;
        heartBeat.type = DataPckTypes::HeartBeat;
    
        this->SendRequest(&heartBeat);
    }
}


void HeartBeatMonitor::MonitorHeartBeat()
{
    if (this->heartBeatStatus != NULL && this->commsStatus != NULL )
    {
        if (this->heartBeatStatus->HeartBeatReceived)
        { 
            this->SendToLogger("Heartbeat received\n\r\0");
            this->heartBeatStatus->HeartBeatReceived = false;
        }
        else
        {
            this->SendToLogger("Heartbeat not received\n\r\0");
            this->commsStatus->mode = CommTypes::Synching;
        }
    }
}

void HeartBeatMonitor::SendToLogger(char* data)
{
    if (this->debugLogger != NULL)
    {
        this->debugLogger->Log(data);   
    }
}

void HeartBeatMonitor::SendRequest(HeartBeatDataPck::HeartBeatData* heartBeat)
{
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
        if (!(this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)heartBeat, sizeof(HeartBeatDataPck::HeartBeatData),false))
        {
            
        }
    }
}

