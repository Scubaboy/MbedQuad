#include "I2CBusCtrl.h"


I2CBusCtrl::I2CBusCtrl(int numOfBusUsers, I2C* bus)
{
    if (bus != NULL)
    {
        this->bus = bus;   
        this->numOfBusUsers = numOfBusUsers;
        this->userHasCtrl = BusCtrlUsers::UnSigned;
        this->busUsersAdded = 0;
    }
    else
    {
        this->bus = NULL;   
    }
}

bool I2CBusCtrl::TakeCtrl(BusCtrlUsers::BusUsers busUser)
{
    bool result = false;
    
    if (userHasCtrl == BusCtrlUsers::UnSigned)
    {
        result = true;
        this->userHasCtrl = busUser;
        this->bus->frequency(this->busUsers[busUser].busFrequency);   
    }
        
    return result;
}
bool I2CBusCtrl::ReleaseCtrl(BusCtrlUsers::BusUsers busUser)
{
    bool result = false;
    
    if (this->userHasCtrl == busUser)
    {
        result = true;
        this->userHasCtrl = BusCtrlUsers::UnSigned;  
    }
    
    return result;   
}

int I2CBusCtrl::WriteToBus(BusCtrlUsers::BusUsers busUser, int addr, const char* value, int len, bool repeatStart)
{
    int result = -1;
    
    if (this->userHasCtrl == busUser)
    {
        if (this->bus != NULL)
        {
            result = this->bus->write(addr,value,len,repeatStart);   
        }
    }
    
    return result;
}

int I2CBusCtrl::ReadFromBus(BusCtrlUsers::BusUsers busUser, int addr, char* value, int len)
{
    int result = -1;
    
    if (this->userHasCtrl == busUser)
    {
        if (this->bus != NULL)
        {
            result = this->bus->read(addr,value,len);
        }
    }
    
    return result;
}

int I2CBusCtrl::ReadFromBus(BusCtrlUsers::BusUsers busUser, int devAddr,int regAddr, char* value, int len, bool repeatStart)
{
    int result = -1;
    
    if (this->userHasCtrl == busUser)
    {
        char data = regAddr;
        result = this->WriteToBus(busUser, devAddr, &data, 1,repeatStart);
        result = this->ReadFromBus(busUser, devAddr, value, len);
    }
    
    return result;
}

bool I2CBusCtrl::AddBusUser (BusCtrlUsers::BusUsers busUser, BusCtrlUsers::BusUserConfig* userBusConfig)
{
    bool result = false;

    if (this->busUsersAdded < this->numOfBusUsers)
    {
        result = true;
        this->busUsers[busUser].busFrequency = ((I2CBusCtrlUser::I2CBusUserConfig*)userBusConfig)->busFrequency;
        this->busUsersAdded++;
    }
    
    return result;
}

bool I2CBusCtrl::SetModeAndBits(BusCtrlUsers::BusUsers busUser, int mode, int bits)
{
 
}
 