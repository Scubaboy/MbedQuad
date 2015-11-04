#ifndef PAxisStabaliseConfiguration_H
#define PAxisStabaliseConfiguration_H
#include "P.h"

namespace PAxisStabaliseConfiguration
{
    typedef struct
    {
        P* roll;
        P* pitch;
        P* yaw;
    } PAxisStabaliseConfigStruct;    
}
#endif