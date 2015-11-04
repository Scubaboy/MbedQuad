#ifndef SPIBusCtrl_H
#define SPIBusCtrl_H
#include "mbed.h"
#include "BusCtrl.h"
#include "SPIBusCtrlTypes.h"

class SPIBusCtrl : public BusCtrl
{
    public:
        SPIBusCtrl(int numOfBusUsers, SPI* bus, const SPIBusCtrlTypes::IsSPIBusUser* isSPIBusUser);
        virtual bool TakeCtrl(BusCtrlUsers::BusUsers busUser);
        virtual bool ReleaseCtrl(BusCtrlUsers::BusUsers busUser);
        int WriteToBus(BusCtrlUsers::BusUsers busUser, int value);
        virtual bool AddBusUser (BusCtrlUsers::BusUsers busUser, BusCtrlUsers::BusUserConfig* userBusConfig);
        bool SetModeAndBits(BusCtrlUsers::BusUsers busUser, int mode, int bits);
    private:
        void SelectDevice(BusCtrlUsers::BusUsers busUser);
        void ReleaseDevice(BusCtrlUsers::BusUsers busUser);
    private:
        SPI* bus;
        SPIBusCtrlTypes::SPIBusUserConfig busUsers[10];
        BusCtrlUsers::BusUsers userHasCtrl;
        int numOfBusUsers;
        int busUsersAdded;
        const SPIBusCtrlTypes::IsSPIBusUser* isSPIBusUSer;
        DigitalOut* ctrlPin;
};
#endif