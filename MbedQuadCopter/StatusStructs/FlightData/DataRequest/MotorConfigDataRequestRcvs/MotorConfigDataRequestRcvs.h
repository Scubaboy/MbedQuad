#ifndef MotorConfigDataRequestRcvs_H
#define MotorConfigDataRequestRcvs_H
#include "QuadMotorMappingDefinition.h"
#include "QuadMotorProperties.h"
#include "PWMChannelMotorMapping.h"

namespace MotorConfigDataRequestRcvs
{
     struct MotorConfigRcvs
     {
        QuadMotorMappingDefinition::MotorFrameMapping frameMappings;
        QuadMotorMappingDefinition::MotorProperties motorProperties;
        QuadMotorProperties::GlobalLimits gloabalLimits;
        PWMChannelMotorMapping::ChannelMotorMappingDefinition pwmChannelMotorMapping;  
        bool motorConfigDataReceived;
    };
};
#endif