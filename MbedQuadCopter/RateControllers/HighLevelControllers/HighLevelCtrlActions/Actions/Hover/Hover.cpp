#include "Hover.h"

Hover::Hover(const float targetAltitude,BasePositionController* posCtrl, AltMeterFlightData::AltMeterFlightDataStruct* flightData)
{
    this->targetAltitude = targetAltitude;
    this->flightData = flightData;
}

bool Hover::Compteleted()
{
    
}

BaseResponse::BaseResp* Hover::Run()
{
    response.pitchDegrees = 0.0;
    response.rollDegrees = 0.0;
    response.yawDegrees = 0.0;
    response.throttle = 10; //need to change this value to work with alitutude.
    return &response;
}