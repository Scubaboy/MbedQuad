#ifndef PWMChannelMotorMapping_H
#define PWMChannelMotorMapping_H
#include "PWMTypes.h"
#include "QuadMotorTypes.h"

namespace PWMChannelMotorMapping
{
    struct ChannelMotorMapping
    {
        PWMTypes::PWMChannelDefinition channelDef;  
        PWMTypes::ChannelId channelID;
    };
    
    typedef ChannelMotorMapping ChannelMotorMappingDefinition[QuadMotorTypes::MaxMotors];
  
};
#endif