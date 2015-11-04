#include "PCA9685PWMCtrl.h"
#include "PCA9685Registers.h"
#include "mbed.h"

const int I2CBaseAddr = 0x40 << 1;
const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::PWMCtrl;
const char On = 0x00;
const int NextPWMOutputJmp = 4;

PCA9685PWMCtrl::PCA9685PWMCtrl(PWMChannelMotorMapping::ChannelMotorMappingDefinition* channelMotorMap, I2CBusCtrl* busCtrl)
{
    this->busCtrl = busCtrl;
    this->channelMotorMap = channelMotorMap; 
    this->ready = false; 
}

void PCA9685PWMCtrl::Reset()
{
    if (this->busCtrl != NULL)
    {
        char data[2];
        
        data[0] = PCA9685Registers::MODE1;
        data[1] = 0x00;
        
        int res = this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);   
    } 
}

void PCA9685PWMCtrl::SetPWMFreq(float freq)
{
    if (this->busCtrl != NULL)
    {
        char writeData[2];
        char readMode;
        char newMode;
        int res;
        
        float prescaleval = 25000000;
        prescaleval /= 4096;
        prescaleval /= freq;
        prescaleval -= 1;
 
        char prescale = (char)(floor(prescaleval + 0.5));
        
        this->busCtrl->WriteToBus(busUser, I2CBaseAddr, PCA9685Registers::MODE1 , 1);

        this->busCtrl->ReadFromBus(busUser,I2CBaseAddr,&readMode,1 );
        
        
        //char oldmode = read8(PCA9685_MODE1);
        newMode = (readMode&0x7F) | 0x10; // sleep
        
        writeData[0] = PCA9685Registers::MODE1;
        writeData[1] = newMode;
        
        this->busCtrl->WriteToBus(busUser, I2CBaseAddr, writeData , 2);

        writeData[0] = PCA9685Registers::PRESCALE;
        writeData[1] = prescale;
        
        this->busCtrl->WriteToBus(busUser, I2CBaseAddr, writeData , 2);
        
        writeData[0] = PCA9685Registers::MODE1;
        writeData[1] = readMode;
        
        this->busCtrl->WriteToBus(busUser, I2CBaseAddr, writeData , 2);
        
        //write8(PCA9685_MODE1, oldmode);
        wait_ms(5);
      
        //  This sets the MODE1 register to turn on auto increment.
        writeData[0] = PCA9685Registers::MODE1;
        writeData[1] = readMode | 0xa1;
        
        this->busCtrl->WriteToBus(busUser, I2CBaseAddr, writeData , 2);
    }
}

bool PCA9685PWMCtrl::SetPWM(QuadMotorTypes::MotorID motorId, short val)
{
    bool result = false;
    
    if (this->busCtrl != NULL)
    {
        char data[5];
        
        PWMChannelMotorMapping::ChannelMotorMapping mapping = (*this->channelMotorMap)[motorId];
        
        if (val >= mapping.channelDef.channelOutMin && val <= mapping.channelDef.channelOutMax)
        {
            data[0] = PCA9685Registers::LED0_ON_L+(NextPWMOutputJmp*mapping.channelID);
            data[1] = On;
            data[2] = On>>8;
            data[3] = val;
            data[4] = val>>8;
        
            if (this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , sizeof(data)) == 0)
            {
                result = true;   
            }
        }
    }
    
    return result;
}