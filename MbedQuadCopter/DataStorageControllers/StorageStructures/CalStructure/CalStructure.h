#ifndef CalStructure_H
#define CalStructure_H
#include "GyroTypes.h"
#include "XYZAxisMinMax.h"
#include "mbed.h"

namespace CalStructure
{
    typedef struct
    {
        time_t timeStamp;
        GyroTypes::GyroCalData gyroCal;
        XYZAxisMinMax::XYZAxisMinMaxCalData accelCal;
        XYZAxisMinMax::XYZAxisMinMaxCalData magCal;
        
    } CalMemStructure;  
};
#endif