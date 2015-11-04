#ifndef P_H
#define P_H
#include "PControlData.h"

class P
{
    public:
        P(PControlData::PControlDataStruct* pStateData);
        float Gain() const;
        void CalculateP(float error);
    private:
       PControlData::PControlDataStruct* pStateData; 
};      
#endif