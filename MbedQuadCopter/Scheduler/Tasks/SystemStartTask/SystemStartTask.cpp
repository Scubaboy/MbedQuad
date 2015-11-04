#include "SystemStartTask.h"
#include "mbed.h"
#include "SystemConfig.h"

SystemStartTask::SystemStartTask(SystemStatusAck::SystemStatusAckStruct* systemStatusAck)
{
    this->sentSystemId = false; 
    this->systemStatusAck = systemStatusAck;
    this->msgPosted = false;   
}


void SystemStartTask::ExecuteTask(int interval)
{
    if (!this->sentSystemId)
    {        
        if (!this->systemStatusAck->acked)
        {
            if (!this->msgPosted)
            {
                SystemIdDataPck::SystemId systemId;
                systemId.type = DataPckTypes::SystemId;
                systemId.platform = SystemConfig::Platform;
                systemId.telemtryFeeds = SystemConfig::TelemetryFeeds;
                systemId.gpsMsgFormat = SystemConfig::GPSMsgFormat;
                systemId.altimeterOptions = SystemConfig::AltimeterOptions;
                systemId.imu = SystemConfig::IMU;
                systemId.quadID = SystemConfig::SystemID;
                systemId.systemID = SystemConfig::SystemID;
                systemId.ackRequired = 1;
                this->SendRequest(&systemId);
            }
        }
        else
        {
            this->sentSystemId = true;
            this->systemStatusAck->acked = false;
        }
    }
}


void SystemStartTask::Init()
{
    
}

void SystemStartTask::SendRequest(SystemIdDataPck::SystemId* systemId)
{
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
       this->msgPosted = (this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)systemId, sizeof(SystemIdDataPck::SystemId),true);
    }
}