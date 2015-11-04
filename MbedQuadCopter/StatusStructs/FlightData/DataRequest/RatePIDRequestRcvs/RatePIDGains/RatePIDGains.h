#ifndef RatePIDGains_H
#define RatePIDGains_H
#include "RatePIDRequestRcvs.h"

namespace RatePIDGains
{
    struct PIDGainsRcvs
    {
        RatePIDRcvs::RatePIDRcvGains gains;
        bool ratePIDGainsRecvd;
    };  
};
#endif