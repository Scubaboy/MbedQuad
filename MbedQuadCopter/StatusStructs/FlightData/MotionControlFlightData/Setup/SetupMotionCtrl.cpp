#include "SetupMotionCtrl.h"

void SetupMotionCtrl::Configure(MotionControlFlightData::MotionControlFlightDataStruct* motionCtrl, 
               PIDControlData::PIDControlDataStruct* rateRoll,
               PIDControlData::PIDControlDataStruct* ratePitch,
               PIDControlData::PIDControlDataStruct* rateYaw, 
               PControlData::PControlDataStruct* stabalisedRoll,
               PControlData::PControlDataStruct* stabalisedPitch,
               PControlData::PControlDataStruct* stabalisedYaw)
{
    motionCtrl->rateRoll = rateRoll;
    motionCtrl->ratePitch = ratePitch;
    motionCtrl->rateYaw = rateYaw;
    motionCtrl->stabalisedRoll = stabalisedRoll;
    motionCtrl->stabalisedPitch = stabalisedPitch;
    motionCtrl->stabalisedYaw = stabalisedYaw;
}