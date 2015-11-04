#include "SysConfigCtrl.h"
#include "MTKV11GPS.h"
#include "mbed.h"

void SysCtrlConfigCtrl::CreateGPS(SystemConfigurationTypes::GPSOption gpsOption)
{
   switch (gpsOption)
   {
       case SystemConfigurationTypes::GPSMtkv11:
       {
            selectedSysConf->gpsConfigCtrl =  new MTKV11GPS(p28,p27,flightData->gpsPositionData,debugLogger);        
            break;    
       }
   }
}