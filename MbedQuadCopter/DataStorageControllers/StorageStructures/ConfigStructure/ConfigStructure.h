#ifndef ConfigStructure_H
#define ConfigStructure_H
#include "QuadMotorMappingDefinition.h"
#include "QuadMotorProperties.h"
#include "PWMChannelMotorMapping.h"
#include "MPL115A2CalData.h"
#include "mbed.h"

namespace ConfigStructure
{
    typedef struct
    {
        time_t timeStamp;
        QuadMotorMappingDefinition::MotorFrameMapping frameMappings;
        QuadMotorMappingDefinition::MotorProperties motorProperties;
        QuadMotorProperties::GlobalLimits gloabalLimits;
        PWMChannelMotorMapping::ChannelMotorMappingDefinition pwmChannelMotorMapping;
        MPL115A2CalData::CalData altimeterConfig;
    } ConfigMemStructure;
};
#endif