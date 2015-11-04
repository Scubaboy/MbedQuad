#ifndef IMUSensorDataPck_H
#define IMUSensorDataPck_H
#include "DataPckTypeBase.h"
#include "XYZAxisMinMax.h"
#include "GyroTypes.h"


namespace IMUSensorDataPck
{
    struct IMUSensor : public DataPckTypeBase::DataPck
    {
       XYZAxisMinMax::XYZAxisMinMaxCalData accelCal;
       GyroTypes::GyroCalData gyroCal;
       XYZAxisMinMax::XYZAxisMinMaxCalData magCal;
    };  
};
#endif