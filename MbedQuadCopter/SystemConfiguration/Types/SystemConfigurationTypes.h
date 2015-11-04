#ifndef SystemConfigurationTypes_H
#define SystemConfigurationTypes_H

namespace SystemConfigurationTypes
{
    typedef unsigned int PlatformConfiguration;
    typedef unsigned int TelemetryFeeds;
    typedef unsigned int GPSMsgFormat;
    typedef unsigned int IMU;
    typedef unsigned int AltimeterOption;
    typedef unsigned int GPSOption;
    
    const unsigned int Quad = 0;
    const unsigned int Heli = 1;
    
    const unsigned int Xbee = 0;
    const unsigned int GSM = 1;
    const unsigned int XbeeGSM = 2;
    
    const unsigned int MPU9150 = 0;
    const unsigned int DCM = 1;
    const unsigned int MPU9150DCM = 2;
    
    const unsigned int GPSShortMsg = 0;
    const unsigned int GPSLongMsg = 1;
    const unsigned int GPSLongShortMsg = 2;
    
    const unsigned int GPSAltitude = 0;
    const unsigned int AltimeterAltitude = 1;
    const unsigned int GPSAltimeterAltitude = 2; 
    
    const unsigned int GPSMtkv11 = 1;
    
};
#endif