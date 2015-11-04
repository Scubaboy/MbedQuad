#ifndef MagSensorBase_H
#define MagSensorBase_H
#include "SensorBase.h"

class MagSensorBase : public SensorBase
{
    public:
        virtual void UpdateHeadingRaw(float roll, float pitch) = 0;
        virtual void UpdateHeadingScaled(float roll, float pitch) = 0;  
        //virtual void InitSensor() = 0;
        //virtual void GetRawSensorData() = 0;  
        //virtual void GetScaledSensorData() = 0;
};
#endif