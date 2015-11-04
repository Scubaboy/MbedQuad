#ifndef RatePIDRequestRcvs_H
#define RatePIDRequestRcvs_H

namespace RatePIDRcvs
{
    //Allowed rate PID gains request responce.
    struct RatePIDRcvGains
    {
        float proportionalGain;
        float integralGain;
        float differentalGain;
        float maxIntegral;   
    };  
};
#endif