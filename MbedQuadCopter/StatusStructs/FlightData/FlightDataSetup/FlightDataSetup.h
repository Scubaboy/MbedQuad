#ifndef FlightDataSetup_H
#define FlightDataSetup_H
#include "SensorFlightData.h"
#include "QuadMotorConfigFlightData.h"
#include "FlightMode.h"
#include "UserResponce.h"
#include "DataRequest.h"
#include "SystemStatusAck.h"

namespace FlightDataSetup
{
    void Initialise(SensorFlightData::SensorFlightDataStruct* flightData);
    
    void Initialise(QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorFlightData);
    
    void Initialise(FlightMode::FlightModeStruct* flightMode);
    
    void Initialise(UserResponce::UserResponceStruct* userResponce, DataRequestRecv::DataRequestRecvs* dataRequestRcvs, SystemStatusAck::SystemStatusAckStruct* systemStatusAck);
    
    void Configure(SensorFlightData::SensorFlightDataStruct* flightData,
                   AccelFlightData::AccelFlightDataStruct* accelFlightData,
                   AltMeterFlightData::AltMeterFlightDataStruct* altimeterFlightData,
                   GyroFlightData::GyroFlightDataStruct* gyroFlightData,
                   MagFlightData::MagFlightDataStruct* magFlightData,
                   IMUFlightData::IMUFlightDataStruct* imuFlightData,
                   FlightPack3CellData::FlightPack3CellDataStruct* flightPackData,
                   GPSPositionData::GPSPositionDataStruct* gpsPositionData);
};
#endif