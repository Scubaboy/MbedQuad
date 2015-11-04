#ifndef PCA9685PWMCtrl_H
#define PwmController_H
#include "PWMController.h"
#include "I2CBusCtrl.h"
#include "PWMChannelMotorMapping.h"

class PCA9685PWMCtrl : public PWMController
{
    public:
        PCA9685PWMCtrl(PWMChannelMotorMapping::ChannelMotorMappingDefinition* channelMotorMap, I2CBusCtrl* busCtrl);
        virtual void Reset();
        virtual void SetPWMFreq(float freq);
        virtual bool SetPWM(QuadMotorTypes::MotorID motorId, short val);
    private:
        PWMChannelMotorMapping::ChannelMotorMappingDefinition* channelMotorMap;
        I2CBusCtrl* busCtrl;
        bool ready; 
};
#endif