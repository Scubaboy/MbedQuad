#ifndef AccelStatus_H
#define AccelStatus_H
#include "StatusTypes.h"

namespace AccelStatus
{
    struct AccelStatusStruct 
    {
        bool usingDefaultCal;
        bool calibrationComplete;
        bool readyForUse;
        bool failedToSetOffsets;
        bool failedToSetScaleFactors;
        char msg[StatusTypes::StatusMsgStringSize];
    
    };
};
#endif