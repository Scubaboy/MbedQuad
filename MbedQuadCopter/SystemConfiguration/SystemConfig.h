#ifndef SystemConfig_H
#define SystemConfig_H
#include "SystemConfigurationTypes.h"

namespace SystemConfig
{
    const SystemConfigurationTypes::PlatformConfiguration Platform = SystemConfigurationTypes::Quad;
    const SystemConfigurationTypes::TelemetryFeeds TelemetryFeeds = SystemConfigurationTypes::Xbee;
    const SystemConfigurationTypes::GPSMsgFormat GPSMsgFormat = SystemConfigurationTypes::GPSLongShortMsg;
    const SystemConfigurationTypes::IMU IMU = SystemConfigurationTypes::MPU9150DCM;
    const SystemConfigurationTypes::AltimeterOption AltimeterOptions = SystemConfigurationTypes::GPSAltimeterAltitude;
    const int SystemID = 4;
};
#endif