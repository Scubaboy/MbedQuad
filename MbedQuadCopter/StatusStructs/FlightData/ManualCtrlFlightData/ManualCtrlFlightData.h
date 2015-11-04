#ifndef ManualCtrlFlightData_H
#define ManualCtrlFlightData_H

namespace ManualCtrlFlightData
{
    typedef struct
    {
        float requestedRoll;
        float requestedPitch;
        float requestedYaw;
        float requestedThrottle;
    } ManualCtrlFlightDataStruct;
};
#endif