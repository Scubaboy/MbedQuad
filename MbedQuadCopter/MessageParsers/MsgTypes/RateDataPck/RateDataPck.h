#ifndef RateDataPck_H
#define RateDataPck_H
#include "DataPckTypeBase.h"

namespace RateDataPck
{
    struct Rate : public DataPckTypeBase::DataPck
    {
        float proportionalGain;
        float integralGain;
        float differentalGain; 
        int maxIntegral;
    };  
};
#endif