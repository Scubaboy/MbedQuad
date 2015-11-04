#ifndef IMUFlightData_H
#define IMUFlightData_H
#include "AccelFlightData.h"
#include "GyroFlightData.h"
#include "MagFlightData.h"

namespace IMUFlightData
{
  struct IMUFlightDataStruct 
    {
        float pitchAngle;
        float rollAngle;
        float yawAngle; 
        float sinRoll;
        float sinPitch;
        float sinYaw;
        float cosYaw;
        float cosPitch;
        float cosRoll;
        float pitchCentiAngle;
        float rollCentiAngle;
        float yawCentiAngle;
        bool valid;
    };
};
#endif