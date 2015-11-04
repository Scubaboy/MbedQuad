#ifndef PID_H
#define PID_H
#include "PIDControlData.h"
#include "Vector2.h"

class PID
{
    public:
        PID(PIDControlData::PIDControlDataStruct* pidStateData, Vector2f constraints);
        void CalculatePID(float error, float timeDelta);
        void CalculatePI(float error, float timeDelta);
        void CalculateP(float error);
        void CalculateI(float error, float timeDelta);
        void CalculateD(float error, float timeDelta);
        void CalculateLeakyI(float error, float timeDelta, float leak_rate);
        void ResetIntegrator();
    private:
        float ConstrainOuput(float rawInput);
    private:
       PIDControlData::PIDControlDataStruct* pidStateData; 
       Vector2f constraints; 
};
#endif