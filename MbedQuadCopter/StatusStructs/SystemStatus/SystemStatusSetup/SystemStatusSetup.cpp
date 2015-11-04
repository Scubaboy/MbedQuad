#include "SystemStatusSetup.h"
#include "StatusTypes.h"
#include "mbed.h"

void SystemStatusSetup::Initialise(SystemStatus::SystemStatusStruct* statusData)
{
    
    //Initialise gyro StatusData
    memset (statusData->gyroStatus.msg,'\0',StatusTypes::StatusMsgStringSize);
    
    //Initialise accel StatusData
    memset (statusData->accelStatus.msg,'\0',StatusTypes::StatusMsgStringSize);
    
    //Initialise mag StatusData
    memset (statusData->magStatus.msg,'\0',StatusTypes::StatusMsgStringSize);
    
    //Initialise mag StatusData
    memset (statusData->imuStatus.msg,'\0',StatusTypes::StatusMsgStringSize);
    
    //Initialise altimeter StatusData
    memset (statusData->altStatus.msg,'\0',StatusTypes::StatusMsgStringSize);
    
    //Initialise flightpackcheck StatusData
    memset (statusData->flightPackCheckStatus.cellSelectorMsg,'\0',StatusTypes::StatusMsgStringSize);
    
    //Initialise altimeter StatusData
    memset (statusData->flightPackCheckStatus.cellAdcMsg,'\0',StatusTypes::StatusMsgStringSize);      
}