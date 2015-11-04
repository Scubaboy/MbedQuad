#ifndef GyroFlightData_H
#define GyroFlightData_H

namespace GyroFlightData
{
  struct GyroFlightDataStruct 
    {
        float scaledXAxisRate;
        float scaledYAxisRate;
        float scaledZAxisRate; 
        float scaledXAxisRateCentiDegrees;
        float scaledYAxisRateCentiDegrees;
        float scaledZAxisRateCentiDegrees; 
        float rawXAxisRate;
        float rawYAxisRate;
        float rawZAxisRate; 
    };
};
#endif