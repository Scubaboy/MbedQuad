#ifndef BasicThrYawPitchRollMix_h
#define BasicThrYawPitchRollMix_h
#include "BaseThrYawRollPitchMix.h"
#include "PWMChannelMotorMapping.h"
#include "QuadMotorTypes.h"

class BasicThrYawPitchRollMix : public BaseThrYawRollPitchMix
{
    public:
        
        BasicThrYawPitchRollMix(PWMChannelMotorMapping::ChannelMotorMappingDefinition* pwmChannelAtts);
                                
        //Configures motor pitch roll yaw influences.
        virtual void CompileMotorFactors(QuadMotorMappingDefinition::MotorFrameMapping motorFrameMapping);  
        
        //Returns a pwm value for each motor to achieve the desired roll, pitch, yaw and throttle
        //values.
        virtual short* Output(MixerTypes::InputAtts inputsAtts);
        
        //Returns a minimum pwm value for each motor.
        virtual short* OutputMin();
        
        //Returns a pwm for motor spin testing.
        virtual short OutputTest(QuadMotorTypes::MotorID motorId);
        
        //SetupThrottleCurve - used to linearlise thrust output by motors
        //returns true if curve is created successfully
        virtual void SetupThrottleCurve();
    private:
        QuadMotorMappingDefinition::MotorInternalMapping motorInternalDef; 
        PWMChannelMotorMapping::ChannelMotorMappingDefinition* pwmChannelAtts;
        bool throttleCurveSet;
};
#endif