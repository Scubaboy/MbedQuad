#include "ConfigurePStateData.h"
#include "mbed.h"

void ConfigurePStateData::ConfigureStateData(PControlData::PControlDataStruct* pStateData, const float proportionalGain)
{
    if (pStateData != NULL)
    {
        pStateData->proportionalGain = proportionalGain;
    }
}