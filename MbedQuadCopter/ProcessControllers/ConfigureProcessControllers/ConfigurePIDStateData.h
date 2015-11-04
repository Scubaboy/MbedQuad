#ifndef ConfigurePIDStateData_H
#define ConfigurePIDStateData_H
#include "PIDControlData.h"

namespace ConfigurePIDStateData
{
    void ConfigureStateData(PIDControlData::PIDControlDataStruct* pidStateData, const float proportionalGain, const float integralGain, const float differentalGain, const float maxIntegral);  
};
#endif