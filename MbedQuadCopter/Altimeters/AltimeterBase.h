#ifndef AltimeterBase_H
#define AltimeterBase_H
#include "AltMeterTypes.h"

class AltiMeterBase
{
    public:
        virtual void UpdateAltitude() = 0;  
        virtual void UpdatePresure() = 0;
        virtual void UpdateTemperature() = 0;
        virtual bool ReadSensor() = 0;
        virtual void Init() = 0;
        virtual void SetOffsets() = 0;
};
#endif