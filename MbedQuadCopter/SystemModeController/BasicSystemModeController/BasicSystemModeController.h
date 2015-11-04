#ifndef BasicSystemModeController_H
#define BasicSystemModeController_H
#include "SystemModeControllerBase.h"


class BasicSystemModeController : public SystemModeControllerBase
{
    public:
        BasicSystemModeController(FlightMode::FlightModeStruct* flightMode);
        virtual bool SetMode(FlightMode::Modes systemMode);  
        virtual FlightMode::Modes GetMode();
        virtual FlightMode::Modes GetPreviousMode();
    private:
        FlightMode::FlightModeStruct* flightMode;
};
#endif