#ifndef SPIBusCtrlTypes_H
#define SPIBusCtrlTypes_H
#include "BusUsers.h"
#include "mbed.h"

namespace SPIBusCtrlTypes
{    
    typedef bool IsSPIBusUser[BusCtrlUsers::MaxBusUsers];
    
    typedef struct SPIBusUserConfig : public BusCtrlUsers::BusUserConfig
    {
        int busFrequency;
        DigitalOut* ctrlPin;
        int mode;
        int bits;
        bool isLowForCtrl;
        
    }SPIBusUserConfig;
};
#endif