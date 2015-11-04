#include "StatusDataTextEncoder.h"
#include "StatusTypes.h"
#include "mbed.h"

const int StringSize = 200;

StatusDataTextEncoder::StatusDataTextEncoder(const SystemStatus::SystemStatusStruct* systemStatusData, BaseComms* comms)
{
    this->comms = comms;
    this->systemStatusData = systemStatusData; 
}

void StatusDataTextEncoder::Encode()
{
    char localStatusData[StringSize];
    
    if (this->comms != NULL && this->systemStatusData != NULL)
    {
        if (this->comms->CommsEstablished())
        {
            this->comms->SendDataPacket("System Status Data \n\n\0", 21,false);
            this->BuildAccelStatusData(localStatusData);
            this->comms->SendDataPacket(localStatusData, strlen(localStatusData),false); 
            
            this->BuildGyroStatusData(localStatusData);
            this->comms->SendDataPacket(localStatusData, strlen(localStatusData),false);
            
            this->BuildMagStatusData(localStatusData);
            this->comms->SendDataPacket(localStatusData, strlen(localStatusData),false);
            
            this->BuildIMUStatusData(localStatusData);
            this->comms->SendDataPacket(localStatusData, strlen(localStatusData),false);
            
            this->BuildFlightPackCheckCellSelectStatusData(localStatusData);
            this->comms->SendDataPacket(localStatusData, strlen(localStatusData),false);
            
            this->BuildFlightPackCheckCellVoltageReaderStatusData(localStatusData);
            this->comms->SendDataPacket(localStatusData, strlen(localStatusData),false);
        }
    }
}

void StatusDataTextEncoder::Decode()
{

}

void StatusDataTextEncoder::BuildAccelStatusData(char* sendBuffer)
{
    memset(sendBuffer,'\0',StringSize);
    
    if (strlen(this->systemStatusData->accelStatus.msg) > 0)
    {
        sprintf(sendBuffer,"%s\n",this->systemStatusData->accelStatus.msg);
    }
} 

void StatusDataTextEncoder::BuildGyroStatusData(char* sendBuffer)
{
    memset(sendBuffer,'\0',StringSize);
            
    if (strlen(this->systemStatusData->gyroStatus.msg) > 0)
    {
        sprintf(sendBuffer,"%s\n",this->systemStatusData->gyroStatus.msg);
    }
}

void StatusDataTextEncoder::BuildMagStatusData(char* sendBuffer)
{
    memset(sendBuffer,'\0',StringSize);
    
    if (strlen(this->systemStatusData->magStatus.msg) > 0)
    {
        sprintf(sendBuffer,"%s\n",this->systemStatusData->magStatus.msg);
    }
}

void StatusDataTextEncoder::BuildIMUStatusData(char* sendBuffer)
{
    memset(sendBuffer,'\0',StringSize);
    
    if (strlen(this->systemStatusData->imuStatus.msg) > 0)
    {
        sprintf(sendBuffer,"%s\n",this->systemStatusData->imuStatus.msg);
    }
}

void StatusDataTextEncoder::BuildFlightPackCheckCellSelectStatusData(char* sendBuffer)
{
    memset(sendBuffer,'\0',StringSize);
    
    if (strlen(this->systemStatusData->flightPackCheckStatus.cellSelectorMsg) > 0)
    {
        sprintf(sendBuffer,"%s\n",this->systemStatusData->flightPackCheckStatus.cellSelectorMsg);   
    }
}

void StatusDataTextEncoder::BuildFlightPackCheckCellVoltageReaderStatusData(char* sendBuffer)
{
    memset(sendBuffer,'\0',StringSize);
    
    if (strlen(this->systemStatusData->flightPackCheckStatus.cellAdcMsg) > 0)
    {
        sprintf(sendBuffer,"%s\n",this->systemStatusData->flightPackCheckStatus.cellAdcMsg);  
    }
}