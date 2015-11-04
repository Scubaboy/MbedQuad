#ifndef FlightPackCheckStatus_H
#define FlightPackCheckStatus_H
#include "StatusTypes.h"

namespace FlightPackCheckStatus
{
    struct FlightPackCheckStruct 
    {
        char cellSelectorMsg[StatusTypes::StatusMsgStringSize];
        char cellAdcMsg[StatusTypes::StatusMsgStringSize];
        bool configured;  
    };
};
#endif