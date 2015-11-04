#include "QuadMotorController.h"
#include "FlightDataSetup.h"

QuadMotorController::QuadMotorController(QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorConfig,
                                         BaseThrYawRollPitchMix* thrYawRollPitchMixer,
                                         PWMController* pwmCtrl)
{
    this->motorConfig = motorConfig;
    this->pwmCtrl = pwmCtrl;
    this->thrYawRollPitchMixer = thrYawRollPitchMixer;
    this->motorsArmedEnabled = false;
}


        
void QuadMotorController::Init()
{
     //Initialise system motor flight data
    FlightDataSetup::Initialise(motorConfig);
    this->thrYawRollPitchMixer->CompileMotorFactors(motorConfig->frameMappings);
    this->mixerInputAtts.roll = 0;
    this->mixerInputAtts.pitch = 0;
    this->mixerInputAtts.yaw = 0;
    this->mixerInputAtts.throttle = 0;  
}
  
//Arms the motors.
void QuadMotorController::Arm()
{
    this->motorsArmedEnabled = true;
    
    this->OutputMin();
    
    for (int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
    {
        this->motorConfig->motorProperties[motorIter].armed = motorsSetToMin[motorIter];
        this->motorsArmedEnabled &= motorsSetToMin[motorIter];
    }
}

// set_roll, set_pitch, set_yaw, set_throttle
void QuadMotorController::SetRoll(int rollIn)            // range -4500 ~ 4500
{
    this->mixerInputAtts.roll = rollIn;
}

void QuadMotorController::SetPitch(int pitchIn)          // range -4500 ~ 4500
{
    this->mixerInputAtts.pitch = pitchIn;
}

void QuadMotorController::SetYaw(int yawIn)              // range -4500 ~ 4500
{
    this->mixerInputAtts.yaw = yawIn;
}

void QuadMotorController::SetThrottle(int throttleIn)    // range 0 ~ 1000
{
    this->mixerInputAtts.throttle = throttleIn;        
}
        
// Output - sends commands to the motors
void QuadMotorController::Output()
{
    short *motorPWMs;
    
    if (this->motorsArmedEnabled)
    {
        motorPWMs = this->thrYawRollPitchMixer->Output(this->mixerInputAtts);
    
        for (int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
        {
            this->pwmCtrl->SetPWM(static_cast<QuadMotorTypes::MotorID>(motorIter),motorPWMs[motorIter]);   
        }
    }
}

// OutputMin - sends minimum values out to the motors
void QuadMotorController::OutputMin()
{
    short *minMotorPWM;
    
    minMotorPWM = this->thrYawRollPitchMixer->OutputMin();
    
    for (int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
    {
        motorsSetToMin[motorIter] = this->pwmCtrl->SetPWM(static_cast<QuadMotorTypes::MotorID>(motorIter),minMotorPWM[motorIter]);  
    }
}

void QuadMotorController::StopMotors()
{
    short *minMotorPWM;
    
    minMotorPWM = this->thrYawRollPitchMixer->OutputMin();
    
    for (int motorIter = 0; motorIter < QuadMotorTypes::MaxMotors; motorIter++)
    {
        this->pwmCtrl->SetPWM(static_cast<QuadMotorTypes::MotorID>(motorIter),minMotorPWM[motorIter]);   
    }
}

// OutputTest - spin each motor for a moment to allow the user to confirm the motor order and spin direction
void QuadMotorController::OutputTest(int motorID)
{
    this->pwmCtrl->SetPWM(static_cast<QuadMotorTypes::MotorID>(motorID),this->thrYawRollPitchMixer->OutputTest(static_cast<QuadMotorTypes::MotorID>(motorID)));

}


// slow_start - set to true to slew motors from current speed to maximum
// Note: this must be set immediately before a step up in throttle
void QuadMotorController::SlowStart(bool slowStart)
{
    
}

QuadMotorProperties::GlobalLimits* QuadMotorController::Limits()
{
    
}