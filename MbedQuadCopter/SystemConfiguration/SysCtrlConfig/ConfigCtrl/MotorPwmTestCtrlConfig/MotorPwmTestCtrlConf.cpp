#include "SysConfigCtrl.h"
#include "PWMChannelMotorMapping.h"
#include "PWMTypes.h"
#include "PCA9685PWMCtrl.h"

//Temp quad motor PWM channel mapping test
PWMChannelMotorMapping::ChannelMotorMapping tempMotorPWMMapping[4] = 
{
    {600,150,PWMTypes::channel1},
    {600,150,PWMTypes::channel2},
    {600,150,PWMTypes::channel3},
    {600,150,PWMTypes::channel4}
};
    
void SysCtrlConfigCtrl::CreateMotorPwmTestCtrl()
{
    selectedSysConf->pwmTestCtrl = new PCA9685PWMCtrl(&tempMotorPWMMapping, this->busCtrl1);
    selectedSysConf->pwmTestBusCtrl = this->busCtrl1;
  
}