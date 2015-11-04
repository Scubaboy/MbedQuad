#include "BasicSystemModeController.h"

BasicSystemModeController::BasicSystemModeController(FlightMode::FlightModeStruct* flightMode)
{
    this->flightMode = flightMode;    
}


bool BasicSystemModeController::SetMode(FlightMode::Modes systemMode)
{
     this->flightMode->previousMode = this->flightMode->mode;
     this->flightMode->mode = systemMode;   
}
   
FlightMode::Modes BasicSystemModeController::GetMode()
{
    return this->flightMode->mode;
}  

FlightMode::Modes BasicSystemModeController::GetPreviousMode()
{
    return this->flightMode->previousMode;
}   