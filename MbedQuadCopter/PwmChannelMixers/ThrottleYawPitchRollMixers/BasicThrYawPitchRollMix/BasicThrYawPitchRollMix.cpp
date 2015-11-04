#include "BasicThrYawPitchRollMix.h"
#include "AngleConverters.h"
#include "mbed.h"
#include "Range.h"

const int motorSpinTestInc = 10;
const short minThrottle = 100;

BasicThrYawPitchRollMix::BasicThrYawPitchRollMix(PWMChannelMotorMapping::ChannelMotorMappingDefinition* pwmChannelAtts)
{
    this->pwmChannelAtts = pwmChannelAtts;
    this->throttleCurveSet = false;
}

//Configures motor pitch roll yaw influences.
void BasicThrYawPitchRollMix::CompileMotorFactors(QuadMotorMappingDefinition::MotorFrameMapping motorFrameMapping)
{
    for (int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
    {
        motorInternalDef[motorIter].id = motorFrameMapping[motorIter].id; 
        motorInternalDef[motorIter].rollFactor = (float)sin((float)(AngleConverters::DegreesToRadians(motorFrameMapping[motorIter].orientation)));
        motorInternalDef[motorIter].pitchFactor = (float)cos((float)(AngleConverters::DegreesToRadians(motorFrameMapping[motorIter].orientation)));
        motorInternalDef[motorIter].yawFactor = motorFrameMapping[motorIter].yawFactor;
        motorInternalDef[motorIter].testOrder = motorFrameMapping[motorIter].testOrder;
    }
}
        
//Returns a pwm value for each motor to achieve the desired roll, pitch, yaw and throttle
//values.
short* BasicThrYawPitchRollMix::Output(MixerTypes::InputAtts inputsAtts)
{
       short rawMotorRPYPWM[QuadMotorTypes::MaxMotors];
       short scaledMotorRPYPWM[QuadMotorTypes::MaxMotors];  
       
       for (int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
       {
             rawMotorRPYPWM[motorIter] = (inputsAtts.throttle + inputsAtts.yaw)+
                                            (motorInternalDef[motorIter].pitchFactor*(inputsAtts.pitch/2))+
                                            (motorInternalDef[motorIter].rollFactor*(inputsAtts.roll/2));  
       }
       
       //Pitch PWM
       //rawMotorRPYPWM[0] = (inputsAtts.throttle + inputsAtts.yaw)+(inputsAtts.pitch/2);
       //rawMotorRPYPWM[2] = (inputsAtts.throttle + inputsAtts.yaw)-(inputsAtts.pitch/2);
       
       //Roll PWM
       //rawMotorRPYPWM[1] = (inputsAtts.throttle + inputsAtts.yaw)+(inputsAtts.roll/2);
       //rawMotorRPYPWM[3] = (inputsAtts.throttle + inputsAtts.yaw)-(inputsAtts.roll/2);
       
       //Need tp map to PWM range
       // add scaled roll, pitch, constrained yaw and throttle for each motor
       for (int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
       {
            //Constrain to fit into min/max range of pwm channels.
            scaledMotorRPYPWM[motorIter]= Range::Constrain(rawMotorRPYPWM[motorIter], (*this->pwmChannelAtts)[motorIter].channelDef.channelOutThrMin, (*this->pwmChannelAtts)[motorIter].channelDef.channelOutThrMax);
        }
}
        
//Returns a minimum pwm value for each motor.
short* BasicThrYawPitchRollMix::OutputMin()
{
    short minMotorPWM[QuadMotorTypes::MaxMotors];
    
    for(int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
    {
         minMotorPWM[motorIter] = (*this->pwmChannelAtts)[motorIter].channelDef.channelOutThrMin;
    }
    
    return minMotorPWM;   
}
        
//Returns a pwm for motor spin testing.
short BasicThrYawPitchRollMix::OutputTest(QuadMotorTypes::MotorID motorId)
{
    return (*this->pwmChannelAtts)[motorId].channelDef.channelOutThrMin+motorSpinTestInc; 
}
        
//SetupThrottleCurve - used to linearlise thrust output by motors
//returns true if curve is created successfully
void BasicThrYawPitchRollMix::SetupThrottleCurve()
{
    this->throttleCurveSet = false;
}