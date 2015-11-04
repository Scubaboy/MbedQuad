#ifndef SystemIdDataPck_H
#define SystemIdDataPck_H
#include "DataPckTypeBase.h"
#include "SystemConfigurationTypes.h"

namespace SystemIdDataPck
{
    struct SystemId : public DataPckTypeBase::DataPck
    {
        SystemConfigurationTypes::PlatformConfiguration platform;
        SystemConfigurationTypes::TelemetryFeeds telemtryFeeds;
        SystemConfigurationTypes::GPSMsgFormat gpsMsgFormat;
        SystemConfigurationTypes::AltimeterOption altimeterOptions;
        SystemConfigurationTypes::IMU imu;
        int systemID;
    };  
};
#endif