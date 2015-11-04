#ifndef FlightDataDataPck_H
#define FlightDataDataPck_H
#include "DataPckTypeBase.h"
#include "AccelFlightData.h"
#include "AltMeterFlightData.h"
#include "GyroFlightData.h"
#include "IMUFlightData.h"
#include "MagFlightData.h"

namespace FlightDataDataPck
{
    struct FlightData : public DataPckTypeBase::DataPck
    {
        AccelFlightData::AccelFlightDataStruct accelFlightData;
        AltMeterFlightData::AltMeterFlightDataStruct altimeterFlightData;
        GyroFlightData::GyroFlightDataStruct gyroFlightData;
        MagFlightData::MagFlightDataStruct magFlightData;
        IMUFlightData::IMUFlightDataStruct imuFlightData;
    };  
};
#endif