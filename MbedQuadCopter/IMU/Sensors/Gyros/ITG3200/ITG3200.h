#ifndef ITG3200_H
#define ITG3200_H
#include "SensorBase.h"
#include "GyroStatus.h"
#include "SensorCalBase.h"
#include "I2CBusCtrl.h"
#include "GyroFlightData.h"

class ITG3200 : public SensorBase
{
    public:
        ITG3200(SensorCalBase* calObji,I2CBusCtrl* busCtrli, GyroStatus::GyroStatusStruct* gyroStatusi, GyroFlightData::GyroFlightDataStruct* flightDatai);
        virtual void InitSensor();
        virtual void GetRawSensorData();  
        virtual void GetScaledSensorData();
    private:
        float RadsPerSecond(float scaledValue);
        bool sensorReady;
        const SensorCalBase* calObj;
        GyroStatus::GyroStatusStruct* gyroStatus;
        GyroFlightData::GyroFlightDataStruct* flightData;
        I2CBusCtrl* busCtrl;
};

#endif