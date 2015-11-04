#ifndef FlightMode_H
#define FlightMode_H

namespace FlightMode
{
    const unsigned int StartUp = 0;
    const unsigned int ConfigCal = 2;
    const unsigned int ArmMotors = 3;
    const unsigned int Active = 4;
    const unsigned int Shutdown = 5;
    const unsigned int PWMRangeTest = 6;
    const unsigned int NotSet = 7;
    const unsigned int CommsSynch = 8;
    
    bool IsValidMode(unsigned int mode);
    
    typedef unsigned int Modes;
 
    struct FlightModeStruct
    {
        Modes mode;  
        Modes previousMode;
    };
};
#endif