#ifndef SensorCalibration_H
#define SensorCalibration_H
#include "BaseCalibrationController.h"
#include "SensorCalBase.h"
#include "UserResponce.h"
#include "XYZAxisMinMax.h"
#include "AltimeterCalBase.h"
#include "MPL115A2CalData.h"

namespace SensorCalibration
{
    typedef struct 
    {
        BaseCalibrationController* gyroCalibrator;
        SensorCalBase* gyroCal;
        GyroTypes::GyroCalData *gyroCalData;
        BaseCalibrationController* magCalibrator;
        SensorCalBase* magCal;
        XYZAxisMinMax::XYZAxisMinMaxCalData *magCalData;
        BaseCalibrationController* accelCalibrator;
        SensorCalBase* accelCal;
        UserResponce::UserResponceStruct* userResponce;
        XYZAxisMinMax::XYZAxisMinMaxCalData *accelCalData;
        BaseCalibrationController* altCalibrator;
        AltimeterCalBase* altCal;
        MPL115A2CalData::CalData *altCalData;
        
    } SensorCalibratorStruct;  
};
#endif