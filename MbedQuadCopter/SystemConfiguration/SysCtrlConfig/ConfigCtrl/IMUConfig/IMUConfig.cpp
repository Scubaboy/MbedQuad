#include "SysConfigCtrl.h"
#include "MPU9150.h"
#include "MPU9150MP4_1.h"
#include "ITG3200.h"
#include "ADXL345.h"
#include "HMC5883L.h"
#include "SensorBase.h"
#include "GyroCalController.h"
#include "AccelCalController.h"
#include "MagCalController.h"
#include "DCM.h"
#include "DefaultSysCalController.h"

void SysCtrlConfigCtrl::CreateIMU(SystemConfigurationTypes::IMU imu)
{
    //Set the IMU
    switch (imu)
    {
        case SystemConfigurationTypes::MPU9150:
        {
            this->selectedSysConf->imuBusCtrl = this->busCtrl2;
            this->selectedSysConf->imu = new MPU9150(new MPU9150MP4_1(this->busCtrl2, this->debugLogger), this->debugLogger, this->flightData, this->imuStatus);
            this->selectedSysConf->calibrationCtrl = NULL;
            this->selectedSysConf->calibrationCtrlBusCtrl = NULL;
            this->selectedSysConf->sensorCalibrators->gyroCalibrator = NULL;
            this->selectedSysConf->sensorCalibrators->accelCalibrator = NULL;
            this->selectedSysConf->sensorCalibrators->magCalibrator = NULL;
            this->debugLogger->Log("Setup of MPU9150 Compelete\n\r\0");
            break;   
        }
        case SystemConfigurationTypes::DCM:
        {
            //Setup flight sensors
            SensorBase* gyro = new ITG3200(selectedSysConf->sensorCalibrators->gyroCal,
                                           this->busCtrl1, 
                                           &this->sysStatus->gyroStatus, 
                                           this->flightData->gyroFlightData);
                                           
            SensorBase* accel = new ADXL345(selectedSysConf->sensorCalibrators->accelCal,
                                            this->busCtrl1, 
                                            &this->sysStatus->accelStatus, 
                                            this->flightData->accelFlightData);
                                            
            MagSensorBase* mag = new HMC5883L(selectedSysConf->sensorCalibrators->magCal,
                                              this->busCtrl1, 
                                              &this->sysStatus->magStatus, 
                                              this->flightData->magFlightData);
              
            gyro->InitSensor();
            accel->InitSensor();
            mag->InitSensor();
    
            //Setup sensor calibration controllers
            GyroCalController* gyroCalController  = new GyroCalController (selectedSysConf->dataStorageController, 
                                                                           selectedSysConf->sensorCalibrators->gyroCalData, 
                                                                           this->flightData->gyroFlightData,
                                                                           selectedSysConf->sensorCalibrators->userResponce, 
                                                                           gyro);
            gyroCalController->SendRequest(this->selectedSysConf->systemComms, &BaseComms::SendDataPacket);
    
            AccelCalController* accelCalController  = new AccelCalController (selectedSysConf->dataStorageController, 
                                                                           selectedSysConf->sensorCalibrators->accelCalData, 
                                                                           this->flightData->accelFlightData,
                                                                           selectedSysConf->sensorCalibrators->userResponce, 
                                                                           accel);
            accelCalController->SendRequest(this->selectedSysConf->systemComms, &BaseComms::SendDataPacket);
    
            MagCalController* magCalController  = new MagCalController (selectedSysConf->dataStorageController, 
                                                                           selectedSysConf->sensorCalibrators->magCalData, 
                                                                           this->flightData->magFlightData,
                                                                           selectedSysConf->sensorCalibrators->userResponce, 
                                                                           mag);  
            magCalController->SendRequest(this->selectedSysConf->systemComms, &BaseComms::SendDataPacket);
            
            this->selectedSysConf->sensorCalibrators->gyroCalibrator = gyroCalController;
            this->selectedSysConf->sensorCalibrators->accelCalibrator = accelCalController;
            this->selectedSysConf->sensorCalibrators->magCalibrator = magCalController;
            selectedSysConf->imu = new DCM(true,
                                           gyro,
                                           accel,
                                           mag,
                                           this->flightData,
                                           this->imuStatus,
                                           this->debugLogger);
            selectedSysConf->imuBusCtrl = this->busCtrl1;
            
            selectedSysConf->calibrationCtrl = new DefaultSysCalController(this->busCtrl1, this->selectedSysConf->sensorCalibrators);
            selectedSysConf->calibrationCtrlBusCtrl = this->busCtrl1;
            this->debugLogger->Log("Setup of DCM Compelete\n\r\0");
            break;   
        } 
    }
}