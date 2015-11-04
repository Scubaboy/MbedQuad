#ifndef I2CBusCtrl_H
#define I2CBusCtrl_H
#include "BusCtrl.h"
#include "mbed.h"
#include "I2CBusCtrlTypes.h"

class I2CBusCtrl : public BusCtrl
{
    public:
        I2CBusCtrl(int numOfBusUsers, I2C* bus);
        virtual bool TakeCtrl(BusCtrlUsers::BusUsers busUser);
        virtual bool ReleaseCtrl(BusCtrlUsers::BusUsers busUser);
        virtual int WriteToBus(BusCtrlUsers::BusUsers busUser, int addr, const char* value, int len, bool repeatStart = false);
        virtual int ReadFromBus(BusCtrlUsers::BusUsers busUser, int addr, char* value, int len);
        virtual int ReadFromBus(BusCtrlUsers::BusUsers busUser, int devAddr,int regAddr, char* value, int len, bool repeatStart = false);
        virtual bool AddBusUser (BusCtrlUsers::BusUsers busUser, BusCtrlUsers::BusUserConfig* userBusConfig);
        virtual bool SetModeAndBits(BusCtrlUsers::BusUsers busUser, int mode, int bits);
    private:
        I2C* bus;
        I2CBusCtrlUser::I2CBusUserConfig busUsers[10];
        BusCtrlUsers::BusUsers userHasCtrl;
        int numOfBusUsers;
        int busUsersAdded;
};
#endif