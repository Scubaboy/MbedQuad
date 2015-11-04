#ifndef PIDTunning_H
#define PIDTunning_H

#include "PositionResponse.h"
#include "BaseHighLevelCtrlAction.h"
#include "ManualCtrlFlightData.h"

class PIDTunning : public BaseHighLevelCtrlAction
{
    public:
        PIDTunning (ManualCtrlFlightData::ManualCtrlFlightDataStruct* manualFlightData);
        virtual bool Compteleted();
        virtual BaseResponse::BaseResp* Run(); 
    private:
        PositionResponse::PositionResp response;  
        ManualCtrlFlightData::ManualCtrlFlightDataStruct* manualFlightData;  
};
#endif