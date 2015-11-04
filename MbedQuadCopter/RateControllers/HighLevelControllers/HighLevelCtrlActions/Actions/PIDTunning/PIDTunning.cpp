#include "PIDTunning.h"

PIDTunning::PIDTunning (ManualCtrlFlightData::ManualCtrlFlightDataStruct* manualFlightData)
{
    this->manualFlightData = manualFlightData;
}

bool PIDTunning::Compteleted()
{
     return false;   
}

BaseResponse::BaseResp* PIDTunning::Run()
{
    response.pitchDegrees = this->manualFlightData->requestedPitch;
    response.rollDegrees = this->manualFlightData->requestedRoll;
    response.yawDegrees = this->manualFlightData->requestedYaw;
    response.throttle = this->manualFlightData->requestedThrottle;
    return &response;
}