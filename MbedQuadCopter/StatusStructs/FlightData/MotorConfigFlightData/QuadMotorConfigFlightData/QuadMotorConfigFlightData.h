#ifndef QuadMotorConfigFlightData_H
#define QuadMotorConfigFlightData_H
#include "QuadMotorMappingDefinition.h"
#include "PWMChannelMotorMapping.h"

namespace QuadMotorConfigFlightData
{
    typedef struct
    {
        QuadMotorMappingDefinition::MotorFrameMapping frameMappings;
        QuadMotorMappingDefinition::MotorInternalMapping internalMappings;
        QuadMotorMappingDefinition::MotorProperties motorProperties;
        QuadMotorProperties::GlobalLimits* gloabalLimits;
        PWMChannelMotorMapping::ChannelMotorMappingDefinition pwmChannelMotorMapping;
        
    } QuadMotorConfigFlightDataStruct;
};
#endif