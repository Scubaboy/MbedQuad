#ifndef QuadMotorMappingDefinition_H
#define QuadMotorMappingDefinition_H
#include "QuadMotorTypes.h"
#include "QuadMotorProperties.h"

namespace QuadMotorMappingDefinition
{
    typedef QuadMotorTypes::MotorInternalCtrlDef MotorInternalMapping[QuadMotorTypes::MaxMotors];
    
    typedef QuadMotorTypes::MotorFrameCtrlDef MotorFrameMapping[QuadMotorTypes::MaxMotors];  
    
    typedef  QuadMotorProperties::MotorProperties MotorProperties[QuadMotorTypes::MaxMotors];
};
#endif