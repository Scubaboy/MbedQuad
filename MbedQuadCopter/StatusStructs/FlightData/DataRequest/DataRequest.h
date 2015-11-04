#ifndef DataRequest_H
#define DataRequest_H
#include "AltimeterDataRequestRcvs.h"
#include "RatePIDGains.h"
#include "MotorConfigDataRequestRcvs.h"
#include "StabPRequestRcvs.h"

namespace DataRequestRecv
{  
    //All system wide data request responces.
    struct DataRequestRecvs
    {
        AltimeterDataRequestRcvs::AltitudeDataRequest altDataReqRcvs;
        RatePIDGains::PIDGainsRcvs rollRatePIDGainsRcvs;
        RatePIDGains::PIDGainsRcvs pitchRatePIDGainsRcvs;
        RatePIDGains::PIDGainsRcvs yawRatePIDGainsRcvs;
        MotorConfigDataRequestRcvs::MotorConfigRcvs motorConfigRcvs;
        StabPRequestRcvs::StabPRcvGains stabalisedRoll;
        StabPRequestRcvs::StabPRcvGains stabalisedPitch;
        StabPRequestRcvs::StabPRcvGains stabalisedYaw;
    };  
};
#endif