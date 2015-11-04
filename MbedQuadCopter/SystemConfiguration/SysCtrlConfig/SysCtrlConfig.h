#ifndef SysCtrlConfig_H
#define SysCtrlConfig_H
#include "IMUBase.h"
#include "PWMController.h"
#include "CellVoltageReaderBase.h"
#include "BaseComms.h"
#include "BaseDataStorageController.h"
#include "SystemConfigurator.h"
#include "SensorCalibration.h"
#include "BusCtrl.h"
#include "PCA95383CellSelector.h"
#include "AltimeterBase.h"
#include "BaseSysCalibrationController.h"
#include "BaseSysConfigController.h"
#include "BaseMotorController.h"
#include "GPSBase.h"

namespace SysCtrlConfig
{
    struct SysCtrlConfigStruct 
    {
        IMUBase* imu;
        BusCtrl* imuBusCtrl;
        PWMController* pwmTestCtrl;
        BusCtrl* pwmTestBusCtrl;
        CellVoltageReaderBase* flightPackMonitor; 
        PCA95383CellSelector* flightPackCellSelector;
        BusCtrl* flightPckMntBusCtrl;
        BaseComms* systemComms;
        BaseDataStorageController* dataStorageController;
        BusCtrl* dataStorageBusCtrl;
        AltiMeterBase* altimeterController;
        BusCtrl* altimeterBusCtrl;
        PWMController* pwmController;
        BaseMotorController* motorController;
        BusCtrl* motorBusCtrl;
        SystemConfigurators::SystemConfiguratorStruct* systemConfigurators;
        SensorCalibration::SensorCalibratorStruct* sensorCalibrators;
        BaseSysCalibrationController* calibrationCtrl;
        BusCtrl* calibrationCtrlBusCtrl;
        BaseSysConfigController* systemConfigCtrl;
        BusCtrl* configurationCtrlBusCtrl;
        GPSBase* gpsConfigCtrl;
    };  
    
    void InitialiseSysCtrlConfig(SysCtrlConfigStruct* sysConfigStruct);     
};
#endif