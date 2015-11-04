#ifndef SensorBase_H
#define SensorBase_H
#include "IMUTypes.h"

class SensorBase
{
    public:
        virtual void InitSensor() = 0;
        virtual void GetRawSensorData() = 0;  
        virtual void GetScaledSensorData() = 0;         
};
#endif
