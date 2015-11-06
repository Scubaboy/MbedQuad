#ifndef SystemStatus_H
#define SystemStatus_H
#include "AccelStatus.h"
#include "GyroStatus.h"
#include "MagStatus.h"
#include "AltimeterStatus.h"
#include "IMUStatus.h"
#include "FlightPackCheckStatus.h"
#include "HeartBeatStatusStruct.h"
#include "CommsStatus.h"
#include "TimeSynchStatus.h"

namespace SystemStatus
{
    enum StatusObjs {Accel, Gyro, Mag,Alt, IMU, PID, FlightPack, CtrlPack};
    
    typedef struct
    {
        AccelStatus::AccelStatusStruct accelStatus;
        GyroStatus::GyroStatusStruct gyroStatus;
        MagStatus::MagStatusStruct magStatus;
        AltimeterStatus::AltimeterStatusStruct altStatus;
        IMUStatus::IMUStatusStruct imuStatus;
        FlightPackCheckStatus::FlightPackCheckStruct flightPackCheckStatus;
        HeartBeatStatus::HeartBeatStatusStruct heartBeatStatus;
        CommsStatus::CommsStatusStruct commsStatus;
        TimeSynchStatus::TimeSynchStatusStruct timeSynchStatus;
    } SystemStatusStruct;
}
#endif
