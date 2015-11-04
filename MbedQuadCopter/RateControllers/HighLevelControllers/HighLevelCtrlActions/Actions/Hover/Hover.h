#ifndef Hover_H
#define Hover_H
#include "BaseHighLevelCtrlAction.h"
#include "PositionResponse.h"
#include "AltMeterFlightData.h"
#include "BasePositionController.h"

class Hover : public BaseHighLevelCtrlAction
{
    public:
        Hover(const float targetAltitude,BasePositionController* posCtrl, AltMeterFlightData::AltMeterFlightDataStruct* flightData);
        virtual bool Compteleted();
        virtual BaseResponse::BaseResp* Run(); 
    private:
        PositionResponse::PositionResp response;
        AltMeterFlightData::AltMeterFlightDataStruct* flightData;
        float targetAltitude;
        BasePositionController* posCtrl;
};
#endif