#include "ConfigurePIDStateData.h"
#include "mbed.h"

void ConfigurePIDStateData::ConfigureStateData(PIDControlData::PIDControlDataStruct* pidStateData, const float proportionalGain, const float integralGain, const float differentalGain, const float maxIntegral)
{
    if (pidStateData != NULL)
    {
        pidStateData->proportionalGain = proportionalGain;
        pidStateData->integralGain = integralGain;
        pidStateData->differentalGain = differentalGain;
        pidStateData->maxIntegral = fabs(maxIntegral);
        pidStateData->lastDerivativeInputForFilter = NAN;
    }
}
