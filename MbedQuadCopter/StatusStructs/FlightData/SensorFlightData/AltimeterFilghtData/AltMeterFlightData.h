#ifndef AltMeterFlightData_H
#define AltMeterFlightData_H
#include "AltMeterTypes.h"

namespace AltMeterFlightData
{
  struct AltMeterFlightDataStruct 
    {
        float pressure;
        float temperature;
        float altitude;
        AltimeterTypes::Modes mode;
          
    };
};
#endif