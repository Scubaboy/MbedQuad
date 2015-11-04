#ifndef SensorCalBase_H
#define SensorCalBase_H
#include "IMUTypes.h"

class SensorCalBase
{
    public:
        virtual const IMUTypes::SensorChannelScales GetChannelScaleFactors() const  = 0;
        virtual const IMUTypes::SensorChannelOffsets GetChannelOffsets() const = 0;
};

#endif