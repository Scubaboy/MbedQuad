#ifndef AltimeterStatus_H
#define AltimeterStatus_H
#include "StatusTypes.h"

namespace AltimeterStatus
{
    struct AltimeterStatusStruct 
    {
        bool readyForUse;
        bool calibrationComplete;
        char msg[StatusTypes::StatusMsgStringSize];
    
    };
};
#endif