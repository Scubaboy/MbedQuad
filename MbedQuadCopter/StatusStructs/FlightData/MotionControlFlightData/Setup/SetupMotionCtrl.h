#ifndef SetupMotionCtrl_H
#define SetupMotionCtrl_H
#include "MotionControlFlightData.h"
#include "PIDControlData.h"
#include "PControlData.h"

namespace SetupMotionCtrl
{
    void Configure(MotionControlFlightData::MotionControlFlightDataStruct* motionCtrl, 
               PIDControlData::PIDControlDataStruct* rateRoll,
               PIDControlData::PIDControlDataStruct* ratePitch,
               PIDControlData::PIDControlDataStruct* rateYaw, 
               PControlData::PControlDataStruct* stabalisedRoll,
               PControlData::PControlDataStruct* stabalisedPitch,
               PControlData::PControlDataStruct* stabalisedYaw);  
};
#endif