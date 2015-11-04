#ifndef PIDControlData_H
#define PIDControlData_H

namespace PIDControlData
{
    struct PIDControlDataStruct 
    {
        float proportionalGain;
        float integralGain;
        float differentalGain;
        int maxIntegral;
        float lastDerivativeInput;
        float lastDerivativeInputForFilter;
        float pidValue;
        float piValue;
        float pValue;
        float iValue;
        float dValue;
        float leakyIValue;
    } ; 
};
#endif