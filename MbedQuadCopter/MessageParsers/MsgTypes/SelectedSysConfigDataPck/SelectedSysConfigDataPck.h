#ifndef SelectedSysConfigDataPck_H
#define SelectedSysConfigDataPck_H

namespace SelectedSysConfigDataPck
{
    struct SelectedSysConfig : public DataPckTypeBase::DataPck
    {
        SystemConfigurationTypes::TelemetryFeeds telemtryFeeds;
        SystemConfigurationTypes::GPSMsgFormat gpsMsgFormat;
        SystemConfigurationTypes::IMU imu;
        SystemConfigurationTypes::AltimeterOption altimeterOption;
        SystemConfigurationTypes::GPSOption gpsOption;
    };  
};

#endif