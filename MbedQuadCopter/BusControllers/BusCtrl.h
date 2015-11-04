#ifndef BusCtrl_H
#define BusCtrl_H
#include "BusUsers.h"

class BusCtrl
{
    public:
        virtual bool TakeCtrl(BusCtrlUsers::BusUsers busUser) = 0;
        virtual bool ReleaseCtrl(BusCtrlUsers::BusUsers busUser) = 0;
        //virtual int WriteToBus(BusCtrlUsers::BusUsers busUser, int addr, const char* value, int len) = 0;
        //virtual int ReadFromBus(BusCtrlUsers::BusUsers busUser, int addr, char* value, int len) = 0;
        virtual bool AddBusUser (BusCtrlUsers::BusUsers busUser, BusCtrlUsers::BusUserConfig* userBusConfig) = 0; 
        //virtual bool SetModeAndBits(BusCtrlUsers::BusUsers busUser, int mode, int bits) = 0;
};
#endif