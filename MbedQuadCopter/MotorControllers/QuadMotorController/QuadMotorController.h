#ifndef QuadMotorController_H
#define QuadMotorController_H
#include "BaseMotorController.h"
#include "QuadMotorConfigFlightData.h"
#include "PWMController.h"
#include "QuadMotorTypes.h"
#include "BaseThrYawRollPitchMix.h"

class QuadMotorController : public BaseMotorController
{
    public:
    
        QuadMotorController(QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorConfig,
                            BaseThrYawRollPitchMix* thrYawRollPitchMixer,
                            PWMController* pwmCtrl);
        
        virtual void Init();
        
        //Arms the motors.
        virtual void Arm();

        // set_roll, set_pitch, set_yaw, set_throttle
        virtual void SetRoll(int rollIn);            // range -4500 ~ 4500
        virtual void SetPitch(int pitchIn);          // range -4500 ~ 4500
        virtual void SetYaw(int yawIn);              // range -4500 ~ 4500
        virtual void SetThrottle(int throttleIn);    // range 0 ~ 1000
        
        // Output - sends commands to the motors
        virtual void Output();

        // OutputMin - sends minimum values out to the motors
        virtual void OutputMin();

        // OutputTest - spin each motor for a moment to allow the user to confirm the motor order and spin direction
        virtual void OutputTest(int motorID);

        // slow_start - set to true to slew motors from current speed to maximum
        // Note: this must be set immediately before a step up in throttle
        virtual void SlowStart(bool slowStart);  
        
        virtual QuadMotorProperties::GlobalLimits* Limits();
    private:
        void StopMotors();
    private:
        QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorConfig;
        PWMController* pwmCtrl;
        BaseThrYawRollPitchMix* thrYawRollPitchMixer;
        MixerTypes::InputAtts mixerInputAtts;
        bool motorsSetToMin[QuadMotorTypes::MaxMotors];
        bool motorsArmedEnabled;
};
#endif