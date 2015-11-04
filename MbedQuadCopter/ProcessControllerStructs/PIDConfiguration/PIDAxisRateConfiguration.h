#ifndef PIDAxisRateConfiguration_H
#define PIDAxisRateConfiguration_H
#include "PID.h"

namespace PIDAxisRateConfiguration
{
    typedef struct
    {
        PID* roll;
        PID* pitch;
        PID* yaw;
    } PIDAxisRateConfigStruct;   
};
#endif