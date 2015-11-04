#include "DefaultSysCalController.h"
#include "mbed.h"

DefaultSysCalController::DefaultSysCalController(BusCtrl* busCtrl, SensorCalibration::SensorCalibratorStruct* sensorCalibrators)
{
    this->sensorCalibrators = sensorCalibrators;
    this->busCtrl = busCtrl;
    this->stage = CalibrationControllerTypes::Gyro;
}


bool DefaultSysCalController::CalibrateSystem()
{
    bool calResult = false;
    
    if (this->sensorCalibrators != NULL);
    {
        this->busCtrl->TakeCtrl(BusCtrlUsers::IMU);
        
            switch(this->stage)
            {
                case CalibrationControllerTypes::Gyro:
                {
                    if (this->sensorCalibrators->gyroCalibrator != NULL)
                    {
                        if (this->sensorCalibrators->gyroCalibrator->RunCalibration())
                        {
                            this->stage = CalibrationControllerTypes::Mag;
                        }
                    }
                    else
                    {
                        this->stage = CalibrationControllerTypes::Mag;
                    }
                    
                    break;   
                }   
                case CalibrationControllerTypes::Mag:
                {
                    if (this->sensorCalibrators->magCalibrator != NULL)
                    {
                        if (this->sensorCalibrators->magCalibrator->RunCalibration())
                        {
                            this->stage = CalibrationControllerTypes::Accel;
                        }
                    }
                    else
                    {
                        this->stage = CalibrationControllerTypes::Accel;
                    }
                    
                    break;   
                }
                case CalibrationControllerTypes::Accel:
                {
                    if (this->sensorCalibrators->accelCalibrator != NULL)
                    {
                        if (this->sensorCalibrators->accelCalibrator->RunCalibration())
                        {
                            this->stage = CalibrationControllerTypes::Altimeter;
                        }
                    }
                    else
                    {
                       this->stage = CalibrationControllerTypes::Altimeter; 
                    }
                    
                    break;   
                }
                case CalibrationControllerTypes::Altimeter:
                {
                    if (this->sensorCalibrators->altCalibrator != NULL)
                    {
                        if (this->sensorCalibrators->altCalibrator->RunCalibration())
                        {
                            this->stage = CalibrationControllerTypes::Complete;
                        }
                    }
                    else
                    {
                       this->stage = CalibrationControllerTypes::Complete; 
                    }
                    break;   
                }
                case CalibrationControllerTypes::Complete:
                {
                    calResult = true; 
                    break;  
                }
            }
        this->busCtrl->ReleaseCtrl(BusCtrlUsers::IMU);
    }
    
    return calResult;
}