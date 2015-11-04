#ifndef Status_H
#define Status_H
#include "SystemStatus.h"

class Status
{
    SystemStatus::SystemStatusStruct* GetStatusObj(SystemStatus::StatusObjs stausObj);
};

#endif