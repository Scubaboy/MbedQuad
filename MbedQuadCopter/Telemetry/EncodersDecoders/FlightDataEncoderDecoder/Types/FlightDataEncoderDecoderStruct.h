#ifndef FlightDataEncoderDecoderStruct_H
#define FlightDataEncoderDecoderStruct_H
#include "AccelFlightData.h"
#include "AltMeterFlightData.h"
#include "GyroFlightData.h"
#include "IMUFlightData.h"
#include "MagFlightData.h"

namespace FlightDataEncoderDecoder
{
    const int stringSize = 200;
        
    struct  FlightDataEncoderDecoderStruct
    {
        AccelFlightData::AccelFlightDataStruct accelFlightData;
        AltMeterFlightData::AltMeterFlightDataStruct altimeterFlightData;
        GyroFlightData::GyroFlightDataStruct gyroFlightData;
        MagFlightData::MagFlightDataStruct magFlightData;
        IMUFlightData::IMUFlightDataStruct imuFlightData;
    } ; 
};
#endif