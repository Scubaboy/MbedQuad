#ifndef BaseThreeAxisFlightData_H
#define BaseThreeAxisFlightData_H

namespace BaseThreeAxisFlightData
{
    struct BaseThreeAxisFlightDataStruct
    {
        float rawXAxis;
        float rawYAxis;
        float rawZAxis;  
        float scaledXAxis;
        float scaledYAxis;
        float scaledZAxis;
    };  
};
#endif
