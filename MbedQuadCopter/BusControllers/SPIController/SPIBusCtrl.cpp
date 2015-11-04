#include "SPIBusCtrl.h"

SPIBusCtrl::SPIBusCtrl(int numOfBusUsers, SPI* bus, const SPIBusCtrlTypes::IsSPIBusUser* isSPIBusUser)
{
    if (bus != NULL)
    {
        this->bus = bus;   
        this->numOfBusUsers = numOfBusUsers;
        this->userHasCtrl = BusCtrlUsers::UnSigned;
        this->busUsersAdded = 0;
        this->isSPIBusUSer = isSPIBusUSer;
    }
    else
    {
        this->bus = NULL;   
    }
}

bool SPIBusCtrl::TakeCtrl(BusCtrlUsers::BusUsers busUser)
{
    bool result = false;
    
    if (userHasCtrl == BusCtrlUsers::UnSigned)
    {
        result = true;
        this->userHasCtrl = busUser;
        this->bus->frequency(this->busUsers[busUser].busFrequency);
        this->bus->format(this->busUsers[busUser].mode, this->busUsers[busUser].bits);    
        this->SelectDevice(busUser);
    }
        
    return result;
}
bool SPIBusCtrl::ReleaseCtrl(BusCtrlUsers::BusUsers busUser)
{
    bool result = false;
    
    if (this->userHasCtrl == busUser)
    {
        this->ReleaseDevice(busUser);
        result = true;
        this->userHasCtrl = BusCtrlUsers::UnSigned;  
    }
    
    return result;   
}

void SPIBusCtrl::SelectDevice(BusCtrlUsers::BusUsers busUser)
{
    if (this->userHasCtrl == busUser)
    {
        *this->busUsers[busUser].ctrlPin = !this->busUsers[busUser].isLowForCtrl;
    }
}

void SPIBusCtrl::ReleaseDevice(BusCtrlUsers::BusUsers busUser)
{
    if (this->userHasCtrl == busUser)
    {
        *this->busUsers[busUser].ctrlPin = !this->busUsers[busUser].isLowForCtrl;
    }
}

int SPIBusCtrl::WriteToBus(BusCtrlUsers::BusUsers busUser, int value)
{
    int result = -1;
    
    if (this->userHasCtrl == busUser)
    {
        if (this->bus != NULL)
        {
            result = this->bus->write(value);  
        }
    }
    
    return result;
}

bool SPIBusCtrl::AddBusUser (BusCtrlUsers::BusUsers busUser, BusCtrlUsers::BusUserConfig* userBusConfig)
{
    bool result = false;

    if (this->busUsersAdded < this->numOfBusUsers && this->isSPIBusUSer[busUser])
    {
        result = true;
        this->busUsers[busUser].busFrequency = ((SPIBusCtrlTypes::SPIBusUserConfig*)userBusConfig)->busFrequency;
        this->busUsers[busUser].mode = ((SPIBusCtrlTypes::SPIBusUserConfig*)userBusConfig)->mode;
        this->busUsers[busUser].bits = ((SPIBusCtrlTypes::SPIBusUserConfig*)userBusConfig)->bits;
        this->busUsers[busUser].ctrlPin = ((SPIBusCtrlTypes::SPIBusUserConfig*)userBusConfig)->ctrlPin;
        this->busUsers[busUser].isLowForCtrl = ((SPIBusCtrlTypes::SPIBusUserConfig*)userBusConfig)->isLowForCtrl;
        this->busUsersAdded++;
    }
    
    return result;
}

bool SPIBusCtrl::SetModeAndBits(BusCtrlUsers::BusUsers busUser, int mode, int bits)
{
    bool result = false;
    
    if (this->userHasCtrl == busUser)
    {
        this->bus->format(mode, bits);  
        result = true;
    }
    
    return result;
}