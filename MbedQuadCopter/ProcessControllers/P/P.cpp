#include "P.h"
#include "mbed.h"

P::P(PControlData::PControlDataStruct* pStateData)
{
    if (pStateData != NULL)
    {
        this->pStateData = pStateData;
    }
    else
    {
        this->pStateData = NULL;   
    } 
}

float P::Gain() const
{
     return this->pStateData->proportionalGain;
}

void P::CalculateP(float error)
{
    if (pStateData != NULL)
    {
        this->pStateData->pValue = (float)error * this->pStateData->proportionalGain; 
    }
}