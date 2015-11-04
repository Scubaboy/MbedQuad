#ifndef PWM_Controller_H
#define PWM_Controller_H
#include "QuadMotorTypes.h"

class PWMController
{
    public:
        virtual void Reset() = 0;
        virtual void SetPWMFreq(float freq) = 0;
        virtual bool SetPWM(QuadMotorTypes::MotorID motorId, short val) = 0; 
};
#endif