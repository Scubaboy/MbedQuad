#ifndef FlightCtrlData_H
#define FlightCtrlData_H

#include "ManualCtrlFlightData.h"

namespace FlightCtrlData
{
    typedef struct
    {
        ManualCtrlFlightData::ManualCtrlFlightDataStruct* manualCtrlFlightData;
    } FlightCtrlDataStruct;
};

#endif