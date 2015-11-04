#ifndef BaseThrYawRollPitchMix_H
#define BaseThrYawRollPitchMix_H
#include "QuadMotorTypes.h"
#include "MixerTypes.h"
#include "QuadMotorMappingDefinition.h"

class BaseThrYawRollPitchMix
{
    public:
    
        //Configures motor pitch roll yaw influences.
        virtual void CompileMotorFactors(QuadMotorMappingDefinition::MotorFrameMapping motorFrameMapping) = 0;  
        
        //Returns a pwm value for each motor to achieve the desired roll, pitch, yaw and throttle
        //values.
        virtual short* Output(MixerTypes::InputAtts inputsAtts) = 0;
        
        //Returns a minimum pwm value for each motor.
        virtual short* OutputMin() = 0;
        
        //Returns a pwm for motor spin testing.
        virtual short OutputTest(QuadMotorTypes::MotorID motorId) = 0;
        
        //SetupThrottleCurve - used to linearlise thrust output by motors
        //returns true if curve is created successfully
        virtual void SetupThrottleCurve() = 0;
};
#endif