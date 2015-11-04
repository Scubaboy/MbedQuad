#ifndef ConfigurePStateData_H
#define ConfigurePStateData_H
#include "PControlData.h"

namespace ConfigurePStateData
{
    void ConfigureStateData(PControlData::PControlDataStruct* pStateData, const float proportionalGain);  
};
#endif