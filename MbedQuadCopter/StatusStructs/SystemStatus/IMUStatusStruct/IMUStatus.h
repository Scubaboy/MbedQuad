#ifndef IMUStatus_H
#define IMUStatus_H
#include "StatusTypes.h"

namespace IMUStatus
{
    struct IMUStatusStruct 
    {
        bool readyForUse;
        char msg[StatusTypes::StatusMsgStringSize];
    };
};
#endif