#ifndef MagFlightData_H
#define MagFlightData_H
#include "BaseThreeAxisFlightData.h"

namespace MagFlightData
{
  struct MagFlightDataStruct : public BaseThreeAxisFlightData::BaseThreeAxisFlightDataStruct
    {  
        float rawHeading;
        float scaledHeading;
    };
};
#endif