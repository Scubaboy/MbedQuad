#include "FlightMode.h"

bool FlightMode::IsValidMode(unsigned int mode)
    {
        return mode == StartUp || 
               mode == ConfigCal || 
               mode == ArmMotors || 
               mode == Active || 
               mode == Shutdown || 
               mode == PWMRangeTest || 
               mode == NotSet || 
               mode == CommsSynch;  
    };