#ifndef MagStatus_H
#define MagStatus_H
#include "StatusTypes.h"

namespace MagStatus
{
    struct MagStatusStruct 
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