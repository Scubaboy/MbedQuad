#ifndef MotionControlFlightData_H
#define MotionControlFlightData_H
#include "PIDControlData.h"
#include "PControlData.h"

namespace MotionControlFlightData
{
    typedef struct
    {
        PIDControlData::PIDControlDataStruct* rateRoll;
        PIDControlData::PIDControlDataStruct* ratePitch;
        PIDControlData::PIDControlDataStruct* rateYaw; 
        PControlData::PControlDataStruct* stabalisedRoll;
        PControlData::PControlDataStruct* stabalisedPitch;
        PControlData::PControlDataStruct* stabalisedYaw;
    } MotionControlFlightDataStruct;  
};
#endif