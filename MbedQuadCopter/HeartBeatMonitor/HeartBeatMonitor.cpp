#include "HeartBeatMonitor.h"
#include "mbed.h"
#include "DataPckTypes.h"


HeartBeatMonitor::HeartBeatMonitor(HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus, SystemModeControllerBase* modeController, DebugLoggerBase* debugLogger)
{
     this->heartBeatStatus = heartBeatStatus;
     this->modeController = modeController;
     this->debugLogger = debugLogger;
}

void HeartBeatMonitor::StartSend()
{
    this->heatBeatTimer.attach(this,&HeartBeatMonitor::SendHeartBeat,10.0);
}

void HeartBeatMonitor::SendHeartBeat()
{
    if (this->modeController != NULL && this->modeController->GetMode() != FlightMode::CommsSynch)
    {
        this->SendToLogger("Heartbeat sent\n\r\0");
        HeartBeatDataPck::HeartBeatData heartBeat;
        heartBeat.type = DataPckTypes::HeartBeat;
    
        this->SendRequest(&heartBeat);
        this->heatBeatTimer.attach(this,&HeartBeatMonitor::SendHeartBeat,10.0);
    }
}

void HeartBeatMonitor::StartMonitor()
{
    this->heatBeatTimer.attach(this,&HeartBeatMonitor::MonitorHeartBeat,60.0);
}

void HeartBeatMonitor::MonitorHeartBeat()
{
    this->heatBeatTimer.detach();
    
    if (this->heartBeatStatus != NULL && this->modeController != NULL)
    {
        if (this->heartBeatStatus->HeartBeatReceived)
        { 
            this->SendToLogger("Heartbeat received\n\r\0");
            this->heatBeatTimer.attach(this,&HeartBeatMonitor::MonitorHeartBeat,60.0);
            this->heartBeatStatus->HeartBeatReceived = false;
        }
        else
        {
            this->SendToLogger("Heartbeat not received\n\r\0");
            this->modeController->SetMode(FlightMode::CommsSynch); 
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

