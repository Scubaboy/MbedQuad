#ifndef SensorFlightData_H
#define SensorFlightData_H
#include "AccelFlightData.h"
#include "AltMeterFlightData.h"
#include "GyroFlightData.h"
#include "IMUFlightData.h"
#include "MagFlightData.h"
#include "FlightPack3CellData.h"
#include "GPSPositionData.h"

namespace SensorFlightData
{
    typedef struct
    {
        AccelFlightData::AccelFlightDataStruct* accelFlightData;
        AltMeterFlightData::AltMeterFlightDataStruct* altimeterFlightData;
        GyroFlightData::GyroFlightDataStruct* gyroFlightData;
        MagFlightData::MagFlightDataStruct* magFlightData;
        IMUFlightData::IMUFlightDataStruct* imuFlightData;
        FlightPack3CellData::FlightPack3CellDataStruct* flightPackData;
        GPSPositionData::GPSPositionDataStruct* gpsPositionData;
    } SensorFlightDataStruct;
};

#endif