#ifndef ADXL345_H
#define ADXL345_H
#include "SensorBase.h"
#include "SensorCalBase.h"
#include "AccelStatus.h"
#include "I2CBusCtrl.h"
#include "AccelFlightData.h"

class ADXL345 : public SensorBase
{
    public:
        ADXL345(SensorCalBase* calObj,I2CBusCtrl* BusCtrl, AccelStatus::AccelStatusStruct* accelStatus, AccelFlightData::AccelFlightDataStruct* flightData);
        virtual void InitSensor();
        virtual void GetRawSensorData();  
        virtual void GetScaledSensorData();
    private:
        bool sensorReady;
        const SensorCalBase* calObj;
        AccelStatus::AccelStatusStruct* accelStatus;
        AccelFlightData::AccelFlightDataStruct* flightData;
        I2CBusCtrl* busCtrl;
};
#endif