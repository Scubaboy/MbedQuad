#ifndef SystemConfigurator_H
#define SystemConfigurator_H
#include "BaseConfigurator.h"
#include "QuadMotorConfigFlightData.h"

namespace SystemConfigurators
{
    typedef struct 
    {
        BaseConfigurator* motorConfig;
        QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorConfigData;
    } SystemConfiguratorStruct;
};
#endif