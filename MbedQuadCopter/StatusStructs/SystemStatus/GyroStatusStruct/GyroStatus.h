#ifndef GyroStatus_H
#define GyroStatus_H
#include "StatusTypes.h"

namespace GyroStatus
{
    struct GyroStatusStruct 
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