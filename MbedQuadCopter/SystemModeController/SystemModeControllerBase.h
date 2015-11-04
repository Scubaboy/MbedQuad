#ifndef SystemModeControllerBase_H
#define SystemModeControllerBase_H
#include "FlightMode.h"

class SystemModeControllerBase
{
    public:
        virtual bool SetMode(FlightMode::Modes systemMode) = 0;
        virtual FlightMode::Modes GetMode() = 0;
        virtual FlightMode::Modes GetPreviousMode() = 0;
};
#endif