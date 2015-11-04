#ifndef BaseMotorController_H
#define BaseMotorController_H
#include "QuadMotorProperties.h"

class BaseMotorController
{
    public:
        
        virtual void Init() = 0;         
        
        //Arms the motors.
        virtual void Arm() = 0;

        // set_roll, set_pitch, set_yaw, set_throttle
        virtual void SetRoll(int rollIn) = 0;            // range -4500 ~ 4500
        virtual void SetPitch(int pitchIn) = 0;          // range -4500 ~ 4500
        virtual void SetYaw(int yawIn) = 0;              // range -4500 ~ 4500
        virtual void SetThrottle(int throttleIn) = 0;    // range 0 ~ 1000
        
        // Output - sends commands to the motors
        virtual void Output() = 0;

        // OutputMin - sends minimum values out to the motors
        virtual void OutputMin() = 0;

        // OutputTest - spin each motor for a moment to allow the user to confirm the motor order and spin direction
        virtual void OutputTest(int motorID) = 0;

        // slow_start - set to true to slew motors from current speed to maximum
        // Note: this must be set immediately before a step up in throttle
        virtual void SlowStart(bool slowStart) = 0;
        
        virtual QuadMotorProperties::GlobalLimits* Limits() = 0;
};
#endif