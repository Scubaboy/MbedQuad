#ifndef SysConfigCtrl_H
#define SysConfigCtrl_H
#include "I2CBusCtrl.h"
#include "DebugLoggerBase.h"
#include "SensorFlightData.h"
#include "IMUStatus.h"
#include "SystemConfigurationTypes.h"
#include "SysCtrlConfig.h"
#include "SystemStatus.h"
#include "DataRequest.h"

class SysCtrlConfigCtrl
{
    public:
       SysCtrlConfigCtrl(I2CBusCtrl* busCtrl1, 
                         I2CBusCtrl* busCtrl2,
                         DebugLoggerBase* debugLogger, 
                         SensorFlightData::SensorFlightDataStruct* flightData,
                         QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorConfigFlightData,
                         SystemStatus::SystemStatusStruct* sysStatus,
                         SysCtrlConfig::SysCtrlConfigStruct* selectedSysConf,
                         DataRequestRecv::DataRequestRecvs* dataRequestRcvs);
                         
        void CreateIMU(SystemConfigurationTypes::IMU imu);   
        void CreateFlightPckMonitor();
        void CreateMotorPwmTestCtrl();
        void CreateMotor();
        void CreateAltimeter(SystemConfigurationTypes::AltimeterOption altOption);
        void ReleaseCalConfigObjects();
        void CreateGPS(SystemConfigurationTypes::GPSOption gpsOption);
    private:
        I2CBusCtrl* busCtrl1;
        I2CBusCtrl* busCtrl2;
        DebugLoggerBase* debugLogger;
        SensorFlightData::SensorFlightDataStruct* flightData;
        SystemStatus::SystemStatusStruct* sysStatus;
        IMUStatus::IMUStatusStruct* imuStatus;
        SysCtrlConfig::SysCtrlConfigStruct* selectedSysConf;
        DataRequestRecv::DataRequestRecvs* dataRequestRcvs;
        QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorConfigFlightData;
        
};
#endif