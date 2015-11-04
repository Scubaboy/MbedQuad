#ifndef BusUsers_H
#define BusUsers_H

namespace BusCtrlUsers
{
    enum BusUsers {IMU=0, SpdCtrl=1, GSMDongle=2, Alt=3,FlightPackChecker=4,SDStorage = 5,PWMCtrl=6, UnSigned=7};
    
    const int MaxBusUsers = 7;
    
    typedef struct BusUserConfig
    {
    }BusUserConfig;
};
#endif