#ifndef QuadMotorsDataPck_H
#define QuadMotorsDataPck_H
#include "DataPckTypeBase.h"
#include "QuadMotorMappingDefinition.h"
#include "QuadMotorProperties.h"
#include "PWMChannelMotorMapping.h"

namespace QuadMotorsDataPck
{
    struct Motors : public DataPckTypeBase::DataPck
    {
        QuadMotorMappingDefinition::MotorFrameMapping frameMappings;
        QuadMotorMappingDefinition::MotorProperties motorProperties;
        QuadMotorProperties::GlobalLimits gloabalLimits;
        PWMChannelMotorMapping::ChannelMotorMappingDefinition pwmChannelMotorMapping;
    };  
};
#endif