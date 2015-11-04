#include "ConfigureSPIBusUsers.h"
#include "BusUsers.h"
#include "mbed.h"

void ConfigureSPIBusUsers::SetupSPIBusUserMapping(SPIBusCtrlTypes::IsSPIBusUser* isSPIBusUser)
{
    if (isSPIBusUser != NULL)
    {
        *isSPIBusUser[BusCtrlUsers::IMU] = false;
        *isSPIBusUser[BusCtrlUsers::SpdCtrl] = false;
        *isSPIBusUser[BusCtrlUsers::GSMDongle] = false;
        *isSPIBusUser[BusCtrlUsers::Alt] = false;
        *isSPIBusUser[BusCtrlUsers::FlightPackChecker] = false;
        *isSPIBusUser[BusCtrlUsers::SDStorage] = true;
        *isSPIBusUser[BusCtrlUsers::UnSigned] = false;
    }
}