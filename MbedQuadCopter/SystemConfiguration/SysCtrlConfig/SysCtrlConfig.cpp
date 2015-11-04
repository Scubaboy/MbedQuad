#include "SysCtrlConfig.h"

void SysCtrlConfig::InitialiseSysCtrlConfig(SysCtrlConfigStruct* sysConfigStruct)
    {
        sysConfigStruct->imu = NULL;
        sysConfigStruct->imuBusCtrl = NULL;
        sysConfigStruct->pwmTestCtrl = NULL;
        sysConfigStruct->pwmTestBusCtrl = NULL;
        sysConfigStruct->flightPackMonitor = NULL; 
        sysConfigStruct->flightPackCellSelector = NULL;
        sysConfigStruct->flightPckMntBusCtrl = NULL;
        sysConfigStruct->systemComms = NULL;
        sysConfigStruct->dataStorageController = NULL;
        sysConfigStruct->dataStorageBusCtrl = NULL;
        sysConfigStruct->altimeterController = NULL;
        sysConfigStruct->altimeterBusCtrl = NULL;
        sysConfigStruct->pwmController = NULL;
        sysConfigStruct->motorController = NULL;
        sysConfigStruct->motorBusCtrl = NULL;
        sysConfigStruct->systemConfigurators = NULL;
        sysConfigStruct->sensorCalibrators = NULL;
        sysConfigStruct->calibrationCtrl = NULL;
        sysConfigStruct->calibrationCtrlBusCtrl = NULL;
        sysConfigStruct->systemConfigCtrl = NULL;
        sysConfigStruct->configurationCtrlBusCtrl = NULL;
        sysConfigStruct->gpsConfigCtrl = NULL;
    }; 