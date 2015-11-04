#ifndef HeartBeatIndicator_H
#define HeartBeatIndicator_H
#include "mbed.h"

class HeartBeatIndicator
{
    public:
       HeartBeatIndicator (PinName heartBeatPin);
       void UpdateHeartBeat();
    private:
        DigitalOut heartBeat;  
};
#endif