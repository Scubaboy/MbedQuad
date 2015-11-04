#include "SysConfigCtrl.h"

SysCtrlConfigCtrl::SysCtrlConfigCtrl(I2CBusCtrl* busCtrl1, 
                         I2CBusCtrl* busCtrl2,
                         DebugLoggerBase* debugLogger, 
                         SensorFlightData::SensorFlightDataStruct* flightData,   
                         QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorConfigFlightData,          
                         SystemStatus::SystemStatusStruct* sysStatus,
                         SysCtrlConfig::SysCtrlConfigStruct* selectedSysConf,
                         DataRequestRecv::DataRequestRecvs* dataRequestRcvs)
{
    this->busCtrl1 = busCtrl1;
    this->busCtrl2 = busCtrl2;
    this->debugLogger = debugLogger;
    this->flightData = flightData;
    this->imuStatus = &sysStatus->imuStatus; 
    this->selectedSysConf = selectedSysConf;
    this->sysStatus = sysStatus;
    this->dataRequestRcvs = dataRequestRcvs;
    this->motorConfigFlightData = motorConfigFlightData;
}